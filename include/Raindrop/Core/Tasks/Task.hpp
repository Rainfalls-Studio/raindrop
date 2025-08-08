#pragma once

#include <functional>
#include <cstdint>
#include "Types.hpp"

namespace Raindrop::Tasks{
    struct Task {
        using Func = std::function<TaskStatus()>;

        uint64_t id;
        int priority; // larger = higher priority
        Func fn;
        std::vector<uint64_t> deps; // ids of tasks this task depends on
        ms retryDelayOnRequest{50}; // default backoff when Retry is returned
        std::string name;

        Task() = default;
        Task(
            uint64_t id_,
            int priority,
            Func function,
            std::vector<uint64_t> dependencies,
            std::string name=""
        );
    };
}