#include "datetime_helper.hpp"

#include <iomanip>

namespace vpn_manager {

std::string FormatTimePoint(
    const userver::storages::postgres::TimePointTz& time_point) {
  auto sys_time = time_point.GetUnderlying();
  std::time_t time_t_value = std::chrono::system_clock::to_time_t(sys_time);
  std::tm tm_value = *std::gmtime(&time_t_value);

  // Format as ISO 8601 (YYYY-MM-DDTHH:MM:SSZ)
  std::ostringstream oss;
  oss << std::put_time(&tm_value, "%Y-%m-%dT%H:%M:%SZ");

  return oss.str();
}

}  // namespace vpn_manager