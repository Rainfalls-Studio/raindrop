#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>

#include "Hook.hpp"
#include "IStage.hpp"
#include "Raindrop/Core/Time/Clock.hpp"
#include "Raindrop/Core/Tasks/TaskHandle.hpp"

namespace Raindrop::Scheduler{
    struct LoopData {
        LoopData() = default;
        
        std::string name;
        Time::Duration period = Time::Duration::zero(); // 0 means unlimited
        int executionPriority = 0;
        std::vector<std::shared_ptr<IStage>> stages;
        std::vector<Hook> hooks;

        struct Runtime {
            LoopData* loop;
            std::atomic<bool> running{true};
            Time::TimePoint lastRunTime;
            Tasks::TaskHandle controller;
        };

        std::shared_ptr<Runtime> runtime;

        template<typename StageT, typename... Args>
        LoopData& addStage(Args&&... args) {
            auto stage = std::make_unique<StageT>(std::forward<Args>(args)...);
            stage->registerHooks(hooks);
            stages.push_back(std::move(stage));
            return *this;
        }

        LoopData& setPeriod(Time::Duration period_) { period = period_; return *this; }
        LoopData& setExecutionPriority(int prio) { executionPriority = prio; return *this; }
    };
}