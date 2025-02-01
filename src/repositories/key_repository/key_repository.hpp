#pragma once

#include <boost/uuid/uuid.hpp>
#include <optional>
#include <string>
#include <userver/components/component_base.hpp>
#include <userver/components/raw_component_base.hpp>
#include <userver/dynamic_config/source.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>
#include "models/key_dto.hpp"
#include "models/user_dto.hpp"

namespace vpn_manager::repositories {
class KeyRepositoryComponent final : public userver::components::ComponentBase {
 public:
  const char* const kKeyTable = "vpn_manager.key";

  static constexpr std::string_view kName = "key-repository-component";

  KeyRepositoryComponent(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context);

  bool KeyExistsByUserId(std::string_view user_id) const;
  bool KeyExistsByName(std::string_view name) const;

  std::optional<KeyDto> GetKeyById(boost::uuids::uuid key_id) const;
  std::optional<KeyDto> GetKeyByName(std::string_view name) const;

  boost::uuids::uuid CreateKey(boost::uuids::uuid user_id,
                               std::string_view name,
                               std::string_view key) const;

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
  userver::dynamic_config::Source config_;
};
}  // namespace vpn_manager::repositories