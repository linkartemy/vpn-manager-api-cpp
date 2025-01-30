#pragma once

#include "helpers/string_convertible_checker.hpp"

#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>

#include <userver/formats/json/value_builder.hpp>

namespace vpn_manager::response {

/**
 * @brief A response class representing successful (normal) data.
 *        Typically analogous to HTTP 200.
 */
class Response {
 public:
  explicit Response(std::string_view data) : data_(data) {}

  [[nodiscard]] std::string ToJson() const {
    std::ostringstream oss;
    oss << R"({"data": )" << data_ << "}";
    return oss.str();
  }

  [[nodiscard]] std::string_view getData() const { return data_; }

 private:
  std::string data_;
};

/**
 * @brief A response class specifically for errors (e.g., not HTTP 200).
 */
class ErrorResponse {
 public:
  explicit ErrorResponse(std::string_view error_message)
      : error_message_(error_message) {}

  template <typename T, typename = std::enable_if_t<is_string_convertible_v<T>>>
  ErrorResponse(std::string_view error_message,
                const std::unordered_map<T, T>& params)
      : error_message_(error_message) {
    for (const auto& [key, value] : params) {
      params_.emplace(ToString(key), ToString(value));
    }
  }

  [[nodiscard]] std::string ToJson() const {
    std::ostringstream oss;
    oss << R"({"error": ")" << error_message_ << R"("})";

    if (!params_.empty()) {
      oss << R"(, "params": {)";
      bool first = true;
      for (const auto& [key, value] : params_) {
        if (!first) oss << ", ";
        first = false;
        oss << R"(")" << key << R"(": ")" << value << R"(")";
      }
      oss << "}";
    }
    oss << "}";
    return oss.str();
  }

  /**
   * @brief Set or overwrite a parameter.
   *        (Useful if you want to add parameters after construction.)
   */
  template <typename T, typename = std::enable_if_t<is_string_convertible_v<T>>>
  void setParam(const std::string& key, const T& value) {
    params_[key] = ToString(value);
  }

  [[nodiscard]] std::string_view getErrorMessage() const {
    return error_message_;
  }

 private:
  std::string error_message_;
  std::unordered_map<std::string, std::string> params_;

  template <typename T>
  static std::string ToString(const T& value) {
    if constexpr (std::is_same_v<T, std::string>) {
      return value;
    } else {
      std::ostringstream oss;
      oss << value;
      return oss.str();
    }
  }
};

}  // namespace vpn_manager::response
