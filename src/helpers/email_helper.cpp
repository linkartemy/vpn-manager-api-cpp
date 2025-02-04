#include "email_helper.hpp"

#include <regex>

namespace vpn_manager {

bool IsEmailValid(const std::string_view email) {
  static const std::regex pattern(
      R"(^[A-Za-z0-9.!#$%&'*+/=?^_`{|}~-]+@(?:(?:[A-Za-z0-9](?:[A-Za-z0-9-]{0,61}[A-Za-z0-9])?\.)+[A-Za-z]{2,})$)");
  return std::regex_match(email.begin(), email.end(), pattern);
}

}  // namespace vpn_manager
