#pragma once

#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <userver/formats/json/value_builder.hpp>
#include <userver/storages/postgres/io/chrono.hpp>
#include <userver/storages/postgres/io/user_types.hpp>
#include <userver/storages/postgres/io/uuid.hpp>
#include <userver/utils/boost_uuid4.hpp>
#include <userver/utils/uuid4.hpp>
#include <vector>
#include "helpers/datetime_helper.hpp"

namespace vpn_manager {

class UserDto {
 public:
  boost::uuids::uuid id;
  std::string username;
  std::string first_name;
  std::string last_name;
  std::optional<std::string> email;
  std::optional<std::string> phone_number;
  userver::storages::postgres::TimePointTz created_at;

  UserDto(boost::uuids::uuid id, std::string username, std::string first_name,
          std::string last_name, std::optional<std::string> email,
          std::optional<std::string> phone_number,
          userver::storages::postgres::TimePointTz created_at)
      : id(std::move(id)),
        username(std::move(username)),
        first_name(std::move(first_name)),
        last_name(std::move(last_name)),
        email(std::move(email)),
        phone_number(std::move(phone_number)),
        created_at(std::move(created_at)) {}

  UserDto(const UserDto&) = default;
  UserDto& operator=(const UserDto&) = default;
  UserDto(UserDto&&) = default;
  UserDto& operator=(UserDto&&) = default;
  ~UserDto() = default;

  userver::formats::json::ValueBuilder ToJson() const {
    userver::formats::json::ValueBuilder obj;
    obj["id"] = fmt::to_string(id);
    obj["username"] = username;
    obj["first_name"] = first_name;
    obj["last_name"] = last_name;
    obj["email"] = email.value_or("");
    obj["phone_number"] = phone_number.value_or("");
    obj["created_at"] = FormatTimePoint(created_at);
    return obj;
  }
};

inline std::string MakeJsonResponse(const std::vector<UserDto>& users) {
  userver::formats::json::ValueBuilder array;
  for (const auto& user : users) {
    array.PushBack(user.ToJson());
  }
  return ToStableString(array.ExtractValue());
}

}  // namespace vpn_manager
