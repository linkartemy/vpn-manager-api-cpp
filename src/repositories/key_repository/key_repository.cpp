#include "key_repository.hpp"
#include "../../constants.hpp"
#include "models/key_dto.hpp"
#include "models/user_dto.hpp"

#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>
#include <userver/dynamic_config/snapshot.hpp>
#include <userver/dynamic_config/storage/component.hpp>
#include <userver/dynamic_config/value.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>
#include <userver/yaml_config/schema.hpp>

namespace vpn_manager::repositories {

KeyRepositoryComponent::KeyRepositoryComponent(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : userver::components::ComponentBase(config, context),
      pg_cluster_(context
                      .FindComponent<userver::components::Postgres>(
                          constants::postgres::kPostgresDBName)
                      .GetCluster()),
      config_(context.FindComponent<userver::components::DynamicConfig>()
                  .GetSource()) {}

bool KeyRepositoryComponent::KeyExistsByUserId(std::string_view user_id) const {
  auto query = fmt::format("SELECT id FROM {} WHERE user_id = $1;", kKeyTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, user_id);
  return !result.IsEmpty();
}

bool KeyRepositoryComponent::KeyExistsByName(std::string_view name) const {
  auto query = fmt::format("SELECT id FROM {} WHERE name = $1;", kKeyTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, name);
  return !result.IsEmpty();
}

std::optional<KeyDto> KeyRepositoryComponent::GetKeyById(
    std::string_view key_id) const {
  auto query = fmt::format(
      "SELECT id, user_id, name, key, created_at FROM {} WHERE id = $1;",
      kKeyTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, key_id);

  if (result.IsEmpty()) {
    return std::nullopt;
  }

  auto [id, user_id, name, key, created_at] = result.AsSingleRow<
      std::tuple<boost::uuids::uuid, boost::uuids::uuid, std::string,
                 std::string, userver::storages::postgres::TimePointTz>>(
      userver::storages::postgres::kRowTag);

  return KeyDto{id, user_id, name, key, created_at};
}

std::optional<KeyDto> KeyRepositoryComponent::GetKeyByName(
    std::string_view name) const {
  auto query = fmt::format(
      "SELECT id, user_id, name, key, created_at FROM {} WHERE name = $1;",
      kKeyTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, name);

  if (result.IsEmpty()) {
    return std::nullopt;
  }

  auto [id, user_id, name_, key, created_at] = result.AsSingleRow<
      std::tuple<boost::uuids::uuid, boost::uuids::uuid, std::string,
                 std::string, userver::storages::postgres::TimePointTz>>(
      userver::storages::postgres::kRowTag);

  return KeyDto{id, user_id, name_, key, created_at};
}

boost::uuids::uuid KeyRepositoryComponent::CreateKey(
    boost::uuids::uuid user_id, std::string_view name,
    std::string_view key) const {
  auto query = fmt::format(
      "INSERT INTO {} (user_id, name, key) VALUES ($1, $2, $3) "
      "RETURNING id;",
      kKeyTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, user_id,
      name, key);

  return result.AsSingleRow<boost::uuids::uuid>();
}

}  // namespace vpn_manager::repositories