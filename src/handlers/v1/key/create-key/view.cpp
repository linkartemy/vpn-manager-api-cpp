#include "view.hpp"
#include "../../../../constants.hpp"
#include "../../../../models/response.hpp"
#include "../../../../models/user_dto.hpp"
#include "../../../../repositories/key_repository/key_repository.hpp"

#include <fmt/format.h>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>

#include <exception>
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/formats/json/serialize.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_status.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/exceptions.hpp>
#include <userver/storages/postgres/io/user_types.hpp>
#include <userver/utils/assert.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/uuid4.hpp>

namespace vpn_manager {

namespace {

class CreateKey final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-v1-create-key";

  CreateKey(const userver::components::ComponentConfig& config,
            const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(component_context
                        .FindComponent<userver::components::Postgres>(
                            constants::postgres::kPostgresDBName)
                        .GetCluster()),
        key_repository_(config, component_context) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto& response = request.GetHttpResponse();

    const auto json_body =
        userver::formats::json::FromString(request.RequestBody());

    if (!json_body.HasMember("name") || !json_body.HasMember("key") ||
        !json_body.HasMember("user_id")) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return response::ErrorResponse(kMissingFields).ToJson();
    }

    boost::uuids::uuid user_id;
    try {
      user_id = userver::utils::BoostUuidFromString(
          json_body["user_id"].As<std::string>());
    } catch (const std::exception& e) {
      LOG_ERROR() << "CreateKey: Invalid user_id: " << e.what();
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return response::ErrorResponse(kInvalidUserId).ToJson();
    } catch (...) {
      LOG_ERROR() << "CreateKey: Invalid user_id";
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return response::ErrorResponse(kInvalidUserId).ToJson();
    }
    std::string name;
    std::string key;
    try {
      name = json_body["name"].As<std::string>();
      key = json_body["key"].As<std::string>();
    } catch (const std::exception& e) {
      LOG_ERROR() << "CreateKey: Unexpected error: " << e.what();
      response.SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return response::ErrorResponse(kUnknownError).ToJson();
    } catch (...) {
      LOG_ERROR() << "CreateKey: Unexpected error";
      response.SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return response::ErrorResponse(kUnknownError).ToJson();
    }

    boost::uuids::uuid key_id;
    try {
      key_id = key_repository_.CreateKey(user_id, name, key);
    } catch (const userver::storages::postgres::UniqueViolation& e) {
      LOG_ERROR() << "CreateKey: Unique constraint violation: " << e.what();
      response.SetStatus(userver::server::http::HttpStatus::kConflict);
      return response::ErrorResponse(kKeyWithSuchNameAlreadyExists).ToJson();
    } catch (const userver::storages::postgres::ForeignKeyViolation& e) {
      LOG_ERROR() << "CreateKey: Foreign key constraint violation: "
                  << e.what();
      response.SetStatus(userver::server::http::HttpStatus::kNotFound);
      return response::ErrorResponse(kUserDoesNotExist).ToJson();
    } catch (const userver::storages::postgres::ClusterError& e) {
      LOG_ERROR() << "CreateKey: Database error: " << e.what();
      response.SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return response::ErrorResponse(kUnknownError).ToJson();
    } catch (const std::exception& e) {
      LOG_ERROR() << "CreateKey: Unexpected error: " << e.what();
      response.SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return response::ErrorResponse(kUnknownError).ToJson();
    } catch (...) {
      LOG_ERROR() << "CreateKey: Unexpected error";
      response.SetStatus(
          userver::server::http::HttpStatus::kInternalServerError);
      return response::ErrorResponse(kUnknownError).ToJson();
    }

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    return response::Response(fmt::format(R"({{"id": "{}"}})", key_id))
        .ToJson();
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
  repositories::KeyRepositoryComponent key_repository_;

 private:
  inline static constexpr std::string_view kMissingFields =
      "Missing required fields";
  inline static constexpr std::string_view kKeyWithSuchNameAlreadyExists =
      "Key with such name already exists";
  inline static constexpr std::string_view kInvalidUserId = "Invalid User ID";
  inline static constexpr std::string_view kUserDoesNotExist =
      "User does not exist";
  inline static constexpr std::string_view kUnknownError = "Unknown error";
};

}  // namespace

void AppendCreateKey(userver::components::ComponentList& component_list) {
  component_list.Append<CreateKey>();
}

}  // namespace vpn_manager