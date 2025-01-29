#include "view.hpp"
#include "../../../../models/user_dto.hpp"

#include <fmt/format.h>

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <iostream>
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

const std::string kLocationHeader = "Location";

class GetUserByUsername final
    : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-v1-get-user-by-username";

  GetUserByUsername(
      const userver::components::ComponentConfig& config,
      const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    const auto& username = request.GetPathArg("username");

    auto result = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "SELECT id, username, first_name, last_name, email, phone_number, "
        "created_at FROM vpn_manager.user "
        "WHERE username = $1",
        username);

    auto& response = request.GetHttpResponse();
    // if (result.IsEmpty()) {
    //   response.SetStatus(userver::server::http::HttpStatus::kNotFound);
    //   return R"({"error": "User not found"})";
    // }

    const auto& row = result[0];
    // UserDto user{
    //     row.As<boost::uuids::uuid>(),
    //     row.As<std::string>(),
    //     row.As<std::string>(),
    //     row.As<std::string>(),
    //     row.As<std::optional<std::string>>(),
    //     row.As<std::optional<std::string>>(),
    //     row.As<userver::storages::postgres::TimePointTz>(),
    // };

    boost::uuids::string_generator gen;
    UserDto user {
      gen(userver::utils::generators::GenerateUuid()),
      "username",
      "first_name",
      "last_name",
      "email",
      "phone_number",
      userver::storages::postgres::TimePointTz(),
    };

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    return userver::formats::json::ToString(user.ToJson().ExtractValue());
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendGetUserByUsername(
    userver::components::ComponentList& component_list) {
  component_list.Append<GetUserByUsername>();
}

}  // namespace vpn_manager