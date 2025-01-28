#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <iomanip>
#include <optional>
#include <sstream>
#include <string>
#include <userver/formats/json/value_builder.hpp>
#include <userver/storages/postgres/io/chrono.hpp>
#include <userver/storages/postgres/io/user_types.hpp>
#include <vector>

namespace vpn_manager {

inline std::string FormatTimePoint(
    const userver::storages::postgres::TimePointTz& time_point) {
  auto sys_time = time_point.GetUnderlying();
  std::time_t time_t_value = std::chrono::system_clock::to_time_t(sys_time);
  std::tm tm_value = *std::gmtime(&time_t_value);

  // Format as ISO 8601 (YYYY-MM-DDTHH:MM:SSZ)
  std::ostringstream oss;
  oss << std::put_time(&tm_value, "%Y-%m-%dT%H:%M:%SZ");

  return oss.str();
}

class UserDto {
 public:
  // Fields
  boost::uuids::uuid id;
  std::string username;
  std::string first_name;
  std::string last_name;
  std::optional<std::string> email;
  std::optional<std::string> phone_number;
  userver::storages::postgres::TimePointTz created_at;

  // Constructor
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

  // Defaulted Rule of Five
  UserDto(const UserDto&) = default;
  UserDto& operator=(const UserDto&) = default;
  UserDto(UserDto&&) = default;
  UserDto& operator=(UserDto&&) = default;
  ~UserDto() = default;

  // JSON Serialization
  userver::formats::json::ValueBuilder ToJson() const {
    userver::formats::json::ValueBuilder obj;
    obj["id"] = boost::uuids::to_string(id);
    obj["username"] = username;
    obj["first_name"] = first_name;
    obj["last_name"] = last_name;
    obj["email"] = email.value_or("");
    obj["phone_number"] = phone_number.value_or("");
    obj["created_at"] = FormatTimePoint(created_at);
    return obj;
  }
};

// Function to create JSON response
inline std::string MakeJsonResponse(const std::vector<UserDto>& users) {
  userver::formats::json::ValueBuilder array;
  for (const auto& user : users) {
    array.PushBack(user.ToJson());
  }
  return ToStableString(array.ExtractValue());
}

}  // namespace vpn_manager
