#pragma once

#include <atomic>
#include <cstdint>
#include "Raindrop/Core/Time/Clock.hpp"

namespace Raindrop::Tasks{
    struct TaskProfile {
        TaskProfile() = default;
        TaskProfile(const TaskProfile& other) : 
            runCount{other.runCount.load()},
            totalNs{other.totalNs.load()}
        {}
        
        std::atomic<uint64_t> runCount{0};
        std::atomic<uint64_t> totalNs{0}; // nanoseconds
        
        void addRun(const Time::Duration& duration) {
            runCount.fetch_add(1, std::memory_order_relaxed);
            totalNs.fetch_add(static_cast<uint64_t>(duration.as<Time::nanoseconds>().count()), std::memory_order_relaxed);
        }

        double avgMs() const {
            auto runs = runCount.load();
            if (!runs) return 0.0;
            return double(totalNs.load()) / 1e6 / double(runs);
        }
    };
}