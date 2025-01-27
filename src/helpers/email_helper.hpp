#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>
#include <vector>

namespace vpn_manager {

bool IsEmailValid(const std::string_view& email);

}  // namespace code_architecture