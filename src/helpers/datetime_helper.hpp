#pragma once

#include <userver/storages/postgres/io/chrono.hpp>

namespace vpn_manager {

std::string FormatTimePoint(
    const userver::storages::postgres::TimePointTz& time_point);

}
