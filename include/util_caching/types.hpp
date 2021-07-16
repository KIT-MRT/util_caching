#pragma once

#include <chrono>


namespace util_caching {

using Clock = std::chrono::steady_clock;
using Time = Clock::time_point;

} // namespace util_caching