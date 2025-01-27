#include "email_helper.hpp"

#include <regex>
#include <string_view>
#include <vector>

namespace vpn_server {

bool IsEmailValid(const std::string_view& email) {
  static const std::regex pattern{
      R"(^[\w.!#$%&'*+/=?^_`{|}~-]+@[\w-]+\.[\w.-]+$)"};
  return std::regex_match(email.begin(), email.end(), pattern);
}

}  // namespace vpn_server