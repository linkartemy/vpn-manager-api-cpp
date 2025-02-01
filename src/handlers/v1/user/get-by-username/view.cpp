#include "view.hpp"
#include "constants.hpp"
#include "models/response.hpp"
#include "models/user_dto.hpp"
#include "repositories/user_repository/user_repository.hpp"

#include <fmt/format.h>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/io/user_types.hpp>
#include <userver/utils/assert.hpp>
#include <userver/utils/uuid4.hpp>

namespace vpn_manager {

namespace {

class GetUserByUsername final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-v1-get-user-by-username";

  GetUserByUsername(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(component_context
                        .FindComponent<userver::components::Postgres>(
                            constants::postgres::kPostgresDBName)
                        .GetCluster()),
        user_repository_(config, component_context) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto& response = request.GetHttpResponse();

    const auto json_body =
        userver::formats::json::FromString(request.RequestBody());
    const auto& username = json_body["username"].As<std::string>();

    auto userOpt = user_repository_.GetUserByUsername(username);
    if (!userOpt.has_value()) {
      response.SetStatus(userver::server::http::HttpStatus::kNotFound);
      return response::ErrorResponse(kUserNotFound).ToJson();
    }
    auto user = userOpt.value();

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    return response::Response(
               userver::formats::json::ToString(user.ToJson().ExtractValue()))
        .ToJson();
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
  repositories::UserRepositoryComponent user_repository_;

 private:
  inline static constexpr std::string_view kUserNotFound = "User not found";
};

}  // namespace

void AppendGetUserByUsername(
    userver::components::ComponentList& component_list) {
  component_list.Append<GetUserByUsername>();
}

}  // namespace vpn_manager