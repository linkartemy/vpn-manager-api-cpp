#include "view.hpp"
#include "../../../../constants.hpp"
#include "../../../../models/key_dto.hpp"
#include "../../../../models/response.hpp"
#include "../../../../repositories/key_repository/key_repository.hpp"

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

class GetKeyById final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-v1-get-key-by-id";

  GetKeyById(const userver::components::ComponentConfig& config,
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
    const auto& key_id = json_body["id"].As<std::string>();

    boost::uuids::uuid key_id_uuid;
    try {
      key_id_uuid = userver::utils::BoostUuidFromString(key_id);
    } catch (const std::exception& e) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return response::ErrorResponse(kInvalidKeyId).ToJson();
    }

    auto keyOpt = key_repository_.GetKeyById(key_id_uuid);
    if (!keyOpt.has_value()) {
      response.SetStatus(userver::server::http::HttpStatus::kNotFound);
      return response::ErrorResponse(kKeyNotFound).ToJson();
    }
    auto key = keyOpt.value();

    response.SetStatus(userver::server::http::HttpStatus::kOk);

    return response::Response(
               userver::formats::json::ToString(key.ToJson().ExtractValue()))
        .ToJson();
  }

  userver::storages::postgres::ClusterPtr pg_cluster_;
  repositories::KeyRepositoryComponent key_repository_;

 private:
  inline static constexpr std::string_view kKeyNotFound = "Key wasn't found";
  inline static constexpr std::string_view kInvalidKeyId = "Invalid Key ID";
};

}  // namespace

void AppendGetKeyById(userver::components::ComponentList& component_list) {
  component_list.Append<GetKeyById>();
}

}  // namespace vpn_manager