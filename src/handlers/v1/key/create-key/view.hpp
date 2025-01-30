#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

namespace vpn_manager {

void AppendCreateKey(userver::components::ComponentList& component_list);

}  // namespace vpn_manager
