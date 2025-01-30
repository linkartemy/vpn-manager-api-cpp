#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "constants.hpp"

#include "handlers/v1/user/create-user/view.hpp"
#include "handlers/v1/user/get-by-username/view.hpp"
#include "repositories/key_repository/key_repository.hpp"
#include "repositories/user_repository/user_repository.hpp"

#include "handlers/v1/key/create-key/view.hpp"

#include "third_party/userver/core/include/userver/clients/dns/component.hpp"
#include "third_party/userver/core/include/userver/dynamic_config/client/component.hpp"
#include "third_party/userver/core/include/userver/dynamic_config/updater/component.hpp"
#include "third_party/userver/core/include/userver/storages/secdist/component.hpp"
#include "third_party/userver/core/include/userver/storages/secdist/provider_component.hpp"
#include "third_party/userver/postgresql/include/userver/storages/postgres/component.hpp"

int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::Postgres>(
              vpn_manager::constants::postgres::kPostgresDBName)
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::server::handlers::TestsControl>()
          .Append<userver::components::HttpClient>()
          .Append<userver::clients::dns::Component>()
          .Append<vpn_manager::repositories::UserRepositoryComponent>()
          .Append<vpn_manager::repositories::KeyRepositoryComponent>();

  vpn_manager::AppendGetUserByUsername(component_list);
  vpn_manager::AppendCreateUser(component_list);
  vpn_manager::AppendCreateKey(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}
