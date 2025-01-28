#pragma once

#include <sstream>
#include <string>
#include <userver/formats/json/value_builder.hpp>
#include <vector>

namespace vpn_manager {
class UserDto {
 public:
  std::string id;
  std::string username;
  std::string first_name;
  std::string last_name;
  std::string email;
  std::string phone_number;
  std::string created_at;

  UserDto(std::string id, std::string username, std::string first_name,
          std::string last_name, std::string email, std::string phone_number, std::string created_at)
      : id(std::move(id)),
        username(std::move(username)),
        first_name(std::move(first_name)),
        last_name(std::move(last_name)),
        email(std::move(email)),
        phone_number(std::move(phone_number)),
        created_at(std::move(created_at)) {}
};

inline std::string MakeJsonResponse(const std::vector<UserDto>& responses) {
  userver::formats::json::ValueBuilder array;
  for (const auto& pair : responses) {
    userver::formats::json::ValueBuilder obj;
    obj["id"] = pair.id;
    obj["username"] = pair.username;
    obj["first_name"] = pair.first_name;
    obj["last_name"] = pair.last_name;
    obj["email"] = pair.email;
    obj["phone_number"] = pair.phone_number;
    obj["created_at"] = pair.created_at;
    array.PushBack(std::move(obj));
  }
  return ToStableString(array.ExtractValue());
}

}  // namespace vpn_manager