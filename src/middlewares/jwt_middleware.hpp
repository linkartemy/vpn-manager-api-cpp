#include "constants.hpp"
#include "helpers/jwt_helper.hpp"

#include <userver/http/header_map.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/utest/using_namespace_userver.hpp>

#include <userver/components/minimal_server_component_list.hpp>
#include <userver/formats/yaml/serialize.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/middlewares/configuration.hpp>
#include <userver/server/middlewares/http_middleware_base.hpp>

#include <jwt-cpp/jwt.h>

namespace vpn_manager::http_middlewares {

class JwtMiddleware final : public server::middlewares::HttpMiddlewareBase {
 public:
  static constexpr std::string_view kName{"jwt-middleware"};

  JwtMiddleware(const server::handlers::HttpHandlerBase&,
                yaml_config::YamlConfig middleware_config)
      : secret_{middleware_config["jwt-secret"].As<std::string>()},
        issuer_{middleware_config["issuer"].As<std::string>("my_issuer")} {}

 private:
  void HandleRequest(server::http::HttpRequest& request,
                     server::request::RequestContext& context) const override {
    const auto auth_header =
        request.GetHeader(constants::headers::kAuthorization);
    if (auth_header.empty()) {
      request.GetHttpResponse().SetStatus(
          server::http::HttpStatus::kUnauthorized);
      return;
    }

    const std::string bearer_prefix = "Bearer ";
    std::string token =
        (auth_header.compare(0, bearer_prefix.size(), bearer_prefix) == 0)
            ? auth_header.substr(bearer_prefix.size())
            : auth_header;

    try {
      vpn_manager::JWTHelper jwt_helper(secret_, issuer_);
      auto decoded = jwt_helper.VerifyToken(token);
      // Optionally, do something with decoded if needed.
    } catch (const std::exception& ex) {
      request.GetHttpResponse().SetStatus(
          server::http::HttpStatus::kUnauthorized);
      request.GetHttpResponse().SetHeader(
          std::string("Error"), std::string("Invalid token: ") + ex.what());
      return;
    }

    Next(request, context);
  }

  const std::string secret_;
  const std::string issuer_;
};

class JwtMiddlewareFactory final
    : public server::middlewares::HttpMiddlewareFactoryBase {
 public:
  static constexpr std::string_view kName{"jwt-middleware"};
  using HttpMiddlewareFactoryBase::HttpMiddlewareFactoryBase;

 private:
  std::unique_ptr<server::middlewares::HttpMiddlewareBase> Create(
      const server::handlers::HttpHandlerBase& handler,
      yaml_config::YamlConfig config) const override {
    return std::make_unique<JwtMiddleware>(handler, std::move(config));
  }

  yaml_config::Schema GetMiddlewareConfigSchema() const override {
    return formats::yaml::FromString(R"(
type: object
description: Configuration for JWT middleware.
additionalProperties: false
properties:
    jwt-secret:
        type: string
        description: Secret key used for verifying JWT tokens.
    issuer:
        type: string
        description: Expected issuer for JWT tokens.
        default: "my_issuer"
)")
        .As<yaml_config::Schema>();
  }
};

}  // namespace vpn_manager::http_middlewares
