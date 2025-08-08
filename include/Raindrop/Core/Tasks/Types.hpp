#pragma once

#include <chrono>

namespace Raindrop::Tasks{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    using ms = std::chrono::milliseconds;

    enum class TaskStatus { Completed, Retry, Failed };
}