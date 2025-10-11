#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <spdlog/spdlog.h>

#include "Hook.hpp"
#include "IStage.hpp"
#include "StageInitHelper.hpp"
#include "LoopStorageRegistry.hpp"
#include "Raindrop/Core/Time/Clock.hpp"
#include "Raindrop/Core/Tasks/TaskHandle.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Scheduler{
    struct LoopData : std::enable_shared_from_this<LoopData>{
        LoopData() = default;
        
        Engine* engine;
        std::string name;
        Time::Duration period = Time::Duration::zero(); // 0 means unlimited
        Tasks::Priority executionPriority = Tasks::Priority::MEDIUM;
        std::vector<std::shared_ptr<IStage>> stages;
        std::vector<Hook> hooks;
        LoopStorageRegistry storage;

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

            std::shared_ptr<LoopData> self = shared_from_this();
            StageInitHelper helper(*engine, self);

            stage->initialize(helper);
            stages.push_back(std::move(stage));

            return *this;
        }

        inline LoopData& setPeriod(Time::Duration period_) {
            period = period_;
            return *this;
        }

        inline LoopData& setExecutionPriority(Tasks::Priority prio) {
            executionPriority = prio;
            return *this;
        }
    };
}