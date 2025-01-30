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

class KeyDto {
 public:
  boost::uuids::uuid id;
  boost::uuids::uuid user_id;
  std::string name;
  std::string key;
  userver::storages::postgres::TimePointTz created_at;

  KeyDto(boost::uuids::uuid id, boost::uuids::uuid user_id, std::string name,
         std::string key, userver::storages::postgres::TimePointTz created_at)
      : id(std::move(id)),
        user_id(std::move(user_id)),
        name(std::move(name)),
        key(std::move(key)),
        created_at(std::move(created_at)) {}

  KeyDto(const KeyDto&) = default;
  KeyDto& operator=(const KeyDto&) = default;
  KeyDto(KeyDto&&) = default;
  KeyDto& operator=(KeyDto&&) = default;
  ~KeyDto() = default;

  userver::formats::json::ValueBuilder ToJson() const {
    userver::formats::json::ValueBuilder obj;
    obj["id"] = fmt::to_string(id);
    obj["user_id"] = fmt::to_string(user_id);
    obj["name"] = name;
    obj["key"] = key;
    obj["created_at"] = FormatTimePoint(created_at);
    return obj;
  }
};

inline std::string MakeJsonResponse(const std::vector<KeyDto>& keys) {
  userver::formats::json::ValueBuilder array;
  for (const auto& key : keys) {
    array.PushBack(key.ToJson());
  }
  return ToStableString(array.ExtractValue());
}

}  // namespace vpn_manager
