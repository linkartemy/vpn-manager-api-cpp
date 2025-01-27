#include "user_repository.hpp"
#include "../../constants.hpp"

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

UserRepositoryComponent::UserRepositoryComponent(
    const userver::components::ComponentConfig& config,
    const userver::components::ComponentContext& context)
    : userver::components::ComponentBase(config, context),
      pg_cluster_(context
                      .FindComponent<userver::components::Postgres>(
                          constants::postgres::kPostgresDBName)
                      .GetCluster()),
      config_(context.FindComponent<userver::components::DynamicConfig>()
                  .GetSource()) {}

bool UserRepositoryComponent::UserExistsById(std::string_view user_id) const {
  auto query = fmt::format("SELECT id FROM {} WHERE id = $1;", kUserTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, user_id);
  return !result.IsEmpty();
}

bool UserRepositoryComponent::UserExistsByUsername(
    std::string_view username) const {
  auto query =
      fmt::format("SELECT id FROM {} WHERE username = $1;", kUserTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, username);
  return !result.IsEmpty();
}

std::string UserRepositoryComponent::GetUserById(
    std::string_view user_id) const {
  auto query = fmt::format(
      "SELECT id, username, first_name, last_name, email, phone_number, "
      "created_at FROM {} WHERE id = $1;",
      kUserTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, user_id);
  return result.AsOptionalSingleRow<std::string>().value_or("unknown");
}

std::string UserRepositoryComponent::GetUserByUsername(
    std::string_view username) const {
  auto query = fmt::format(
      "SELECT id, username, first_name, last_name, email, phone_number, "
      "created_at FROM {} WHERE username = $1;",
      kUserTable);
  auto result = pg_cluster_->Execute(
      userver::storages::postgres::ClusterHostType::kMaster, query, username);
  return result.AsOptionalSingleRow<std::string>().value_or("unknown");
}

}  // namespace vpn_manager::repositories