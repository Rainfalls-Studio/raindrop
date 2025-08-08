#pragma once

#include <atomic>
#include <cstdint>

namespace Raindrop::Tasks{
    struct TaskProfile {
        std::atomic<uint64_t> runCount{0};
        std::atomic<uint64_t> totalNs{0}; // nanoseconds
        
        void addRun(uint64_t ns) {
            runCount.fetch_add(1, std::memory_order_relaxed);
            totalNs.fetch_add(ns, std::memory_order_relaxed);
        }

        double avgMs() const {
            auto runs = runCount.load();
            if (!runs) return 0.0;
            return double(totalNs.load()) / 1e6 / double(runs);
        }
    };
}