#include "view.hpp"
#include "../../../../constants.hpp"
#include "../../../../models/user_dto.hpp"
#include "../../../../repositories/user_repository/user_repository.hpp"

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

class CreateUser final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-v1-create-user";

  CreateUser(const userver::components::ComponentConfig& config,
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

    if (!json_body.HasMember("username") ||
        !json_body.HasMember("first_name") ||
        !json_body.HasMember("last_name")) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return R"({"error": "Missing required fields"})";
    }

    const auto& username = json_body["username"].As<std::string>();
    const auto& first_name = json_body["first_name"].As<std::string>();
    const auto& last_name = json_body["last_name"].As<std::string>();

    std::optional<std::string> email;
    std::optional<std::string> phone_number;

    if (json_body.HasMember("email") && !json_body["email"].IsNull()) {
      email = json_body["email"].As<std::string>();
    }

    if (json_body.HasMember("phone_number") &&
        !json_body["phone_number"].IsNull()) {
      phone_number = json_body["phone_number"].As<std::string>();
    }

    boost::uuids::uuid user_id;
    try {
      user_id = user_repository_.CreateUser(username, first_name, last_name,
                                            email, phone_number);
    } catch (const userver::storages::postgres::UniqueViolation& e) {
      LOG_ERROR() << "CreateUser: Unique constraint violation: " << e.what();
      response.SetStatus(userver::server::http::HttpStatus::kConflict);
      return R"({"error": "Username already exists"})";
    } catch (const userver::storages::postgres::ClusterError& e) {
      LOG_ERROR() << "CreateUser: Database error: " << e.what();
      response.SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return R"({"error": "Database error"})";
    } catch (const std::exception& e) {
      LOG_ERROR() << "CreateUser: Unexpected error: " << e.what();
      response.SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return R"({"error": "Unexpected error"})";
    } catch (...) {
      LOG_ERROR() << "CreateUser: Unexpected error";
      response.SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return R"({"error": "Unexpected error"})";
    }

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    return fmt::format(R"({{"id": "{}"}})", user_id);
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
  repositories::UserRepositoryComponent user_repository_;
};

}  // namespace

void AppendCreateUser(userver::components::ComponentList& component_list) {
  component_list.Append<CreateUser>();
}

}  // namespace vpn_manager