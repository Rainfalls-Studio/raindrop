#pragma once

#include <atomic>
#include <cstdint>
#include "Raindrop/Time/Clock.hpp"

namespace Raindrop::Tasks{
    struct TaskProfile {
        TaskProfile() = default;
        TaskProfile(const TaskProfile& other) : 
            runCount{other.runCount},
            totalNs{other.totalNs},
            lastExecutions(other.lastExecutions),
            cursor{other.cursor}
        {}

        static constexpr size_t EXECUTION_STORAGE_SIZE = 50;
        
        uint64_t runCount;
        uint64_t totalNs;
        std::array<uint32_t, EXECUTION_STORAGE_SIZE> lastExecutions;
        uint32_t cursor;
        
        void addRun(const Time::Duration& duration) {
            const uint64_t d = static_cast<uint64_t>(duration.as<Time::nanoseconds>().count());

            runCount++;
            totalNs += d;
            uint32_t c = cursor = (cursor + 1) % EXECUTION_STORAGE_SIZE;
            lastExecutions[c] = static_cast<uint32_t>(d);
        }

        double avgMs() const {
            if (!runCount) return 0.0;
            return double(totalNs) / 1e6 / double(runCount);
        }
    };
}