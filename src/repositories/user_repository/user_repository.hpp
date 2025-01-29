#pragma once

#include <optional>
#include <string>
#include <userver/components/component_base.hpp>
#include <userver/components/raw_component_base.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>
#include "models/user_dto.hpp"

namespace vpn_manager::repositories {
class UserRepositoryComponent final
    : public userver::components::ComponentBase {
 public:
  const char* const kUserTable = "vpn_manager.user";

  static constexpr std::string_view kName = "user-repository-component";

  UserRepositoryComponent(const userver::components::ComponentConfig& config,
                          const userver::components::ComponentContext& context);

  bool UserExistsById(std::string_view user_id) const;
  bool UserExistsByUsername(std::string_view username) const;

  std::string GetUserById(std::string_view user_id) const;
  std::optional<UserDto> GetUserByUsername(std::string_view username) const;

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
  userver::dynamic_config::Source config_;
};
}  // namespace vpn_manager::repositories