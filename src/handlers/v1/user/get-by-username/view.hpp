#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace vpn_manager {

void AppendGetUserByUsername(
    userver::components::ComponentList& component_list);

}  // namespace vpn_manager
