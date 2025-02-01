#include "phone_helper.hpp"
#include <cctype>
#include <string>

namespace vpn_manager {

bool IsPhoneNumberValid(const std::string_view phone) {
  if (phone.empty() || phone.front() != '+') {
    return false;
  }

  std::string normalized;
  normalized.reserve(phone.size());
  normalized.push_back('+');

  for (size_t i = 1; i < phone.size(); ++i) {
    char c = phone[i];
    if (std::isdigit(c)) {
      normalized.push_back(c);
    } else if (c == ' ' || c == '-' || c == '(' || c == ')' || c == '.') {
      continue;
    } else {
      return false;
    }
  }

  if (normalized.size() == 1 + 11) {
    return true;
  }
  return false;
}

}  // namespace vpn_manager
