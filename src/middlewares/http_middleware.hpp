#include <userver/utest/using_namespace_userver.hpp>

#include <userver/components/minimal_server_component_list.hpp>
#include <userver/formats/yaml/serialize.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/utils/scope_guard.hpp>

#include <userver/server/middlewares/configuration.hpp>
#include <userver/server/middlewares/http_middleware_base.hpp>

namespace vpn_manager::http_middlewares {

// class SomeServerMiddleware final
//     : public server::middlewares::HttpMiddlewareBase {
//  public:
//   // This will be used as a kName for the SimpleHttpMiddlewareFactory
//   static constexpr std::string_view kName{"server-middleware"};

//   // Handler isn't interesting to us, but we could use it if needed.
//   // Or we could implement the factory ourselves instead of using
//   // SimpleHttpMiddlewareFactory, and pass whatever parameters we want.
//   explicit SomeServerMiddleware(const server::handlers::HttpHandlerBase&) {}

//  private:
//   void HandleRequest(server::http::HttpRequest& request,
//                      server::request::RequestContext& context) const override {
//     Next(request, context);
//     request.GetHttpResponse().SetHeader(kCustomServerHeader, "1");
//   }

//   static constexpr http::headers::PredefinedHeader kCustomServerHeader{
//       "X-Some-Server-Header"};
// };

// using SomeServerMiddlewareFactory =
//     server::middlewares::SimpleHttpMiddlewareFactory<SomeServerMiddleware>;

class SomeHandlerMiddleware final
    : public server::middlewares::HttpMiddlewareBase {
 public:
  static constexpr std::string_view kName{"handler-middleware"};

  SomeHandlerMiddleware(const server::handlers::HttpHandlerBase&,
                        yaml_config::YamlConfig middleware_config)
      : header_value_{middleware_config["header-value"].As<std::string>()} {}

 private:
  void HandleRequest(server::http::HttpRequest& request,
                     server::request::RequestContext& context) const override {
    // We will set the header no matter whether downstream succeeded or not.
    //
    // Note that in presence of exceptions other that
    // CustomHandlerException-derived ones, this header would be removed by
    // default userver-provided exceptions handling middleware. If this is
    // undesirable, the middleware should be earlier in the pipeline, or the
    // default exceptions handling behavior could be overridden.
    const utils::ScopeGuard set_header_scope{[this, &request] {
      request.GetHttpResponse().SetHeader(kCustomHandlerHeader, header_value_);
    }};

    Next(request, context);
  }

  static constexpr http::headers::PredefinedHeader kCustomHandlerHeader{
      "X-Some-Handler-Header"};

  const std::string header_value_;
};

class SomeHandlerMiddlewareFactory final
    : public server::middlewares::HttpMiddlewareFactoryBase {
 public:
  static constexpr std::string_view kName{SomeHandlerMiddleware::kName};

  using HttpMiddlewareFactoryBase::HttpMiddlewareFactoryBase;

 private:
  std::unique_ptr<server::middlewares::HttpMiddlewareBase> Create(
      const server::handlers::HttpHandlerBase& handler,
      yaml_config::YamlConfig middleware_config) const override {
    return std::make_unique<SomeHandlerMiddleware>(
        handler, std::move(middleware_config));
  }

  yaml_config::Schema GetMiddlewareConfigSchema() const override {
    return formats::yaml::FromString(R"(
type: object
description: Config for this particular middleware
additionalProperties: false
properties:
    header-value:
        type: string
        description: header value to set for responses
)")
        .As<yaml_config::Schema>();
  }
};
class CustomHandlerPipelineBuilder final
    : public server::middlewares::HandlerPipelineBuilder {
 public:
  using HandlerPipelineBuilder::HandlerPipelineBuilder;

  server::middlewares::MiddlewaresList BuildPipeline(
      server::middlewares::MiddlewaresList server_middleware_pipeline)
      const override {
    auto& pipeline = server_middleware_pipeline;
    pipeline.emplace_back(SomeHandlerMiddleware::kName);

    return pipeline;
  }
};
}  // namespace vpn_manager::http_middlewares