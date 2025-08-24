#include "Raindrop/Core/Scheduler/Scheduler.hpp"
#include "Raindrop/Core/Tasks/TaskManager.hpp"
#include "Raindrop/Core/Time/Clock.hpp"
#include "Raindrop/Engine.hpp"
#include <spdlog/spdlog.h>

namespace Raindrop::Scheduler{
    Tasks::TaskStatus HookResultToTaskStatus(HookResult&& r){
        switch (r.type){
            case HookResult::SKIP: [[fallthrough]];
            case HookResult::CONTINUE: return Tasks::TaskStatus::Completed();
            case HookResult::RETRY_HOOK: return Tasks::TaskStatus::Retry(r.getRetryHook().waitDuration);
        }
        return Tasks::TaskStatus::Failed("Unknown hook result !");
    }

    Scheduler::Scheduler(Engine& engine) :
        _engine{engine},
        _taskManager{engine.getTaskManager()}
    {}

    Scheduler::~Scheduler(){
        shutdown();
    }

    void Scheduler::shutdown(){
        spdlog::info("Shuting down loop scheduler...");
        for (auto [it, loop] : _loops){
            auto runtime = loop->runtime;

            if (runtime){
                spdlog::trace("Stopping loop {}", loop->name);
                runtime->running.store(false);
            }
        }
        _loops.clear();
    }


    Loop Scheduler::createLoop(const std::string& name) {
        auto [it, inserted] = _loops.emplace(name, std::make_shared<LoopData>());

        LoopData& loop = *it->second;
        loop.name = name;
        loop.engine = &_engine;

        return Loop(it->second);
    }

    void Scheduler::run(const Loop& loop){
        assert(loop);
        
        auto data = loop.data();
        auto& runtime = data->runtime;

        // spdlog::trace("Running loop {}", data->name);

        runtime = std::make_shared<LoopData::Runtime>();
        runtime->loop = data.get();
        runtime->lastRunTime = Time::now();

        runtime->controller = _taskManager.createTask(
            [this, data]() -> Tasks::TaskStatus {
                
                auto& loopRuntime = data->runtime;
                if (!loopRuntime->running.load()) return Tasks::TaskStatus::Completed();

                Time::TimePoint now = Time::now();
                if (loopRuntime->loop->period != Time::Duration::zero()) {

                    const Time::Duration& period = loopRuntime->loop->period;
                    Time::Duration elapsed = now - loopRuntime->lastRunTime;

                    if (elapsed < period) {
                        return Tasks::TaskStatus::Retry(period - elapsed); // will be rescheduled with retry delay
                    }

                }
                
                loopRuntime->lastRunTime = now;
                submitLoopIteration(*data);
                return Tasks::TaskStatus::Completed();
            },
            data->executionPriority,
            loop.name() + " - controller"
        );

        _taskManager.submit(runtime->controller);
    }

    Loop Scheduler::getLoop(const std::string& name){
        auto it = _loops.find(name);
        if (it != _loops.end()){
            return Loop(it->second);
        }
        return Loop();
    }

    void Scheduler::submitLoopIteration(LoopData& loop) {
        // Ensure hooks are sorted by phase
        std::sort(loop.hooks.begin(), loop.hooks.end(),
                [](const Hook& a, const Hook& b) {
                    return static_cast<int>(a.phase) > static_cast<int>(b.phase);
                });

        auto controller = loop.runtime->controller;

        Tasks::TaskHandle prev = controller;
        // spdlog::info("running {}", loop.name);

        for (auto& hook : loop.hooks) {
            auto h = _taskManager.createTask(
                [hook] -> Tasks::TaskStatus {return HookResultToTaskStatus(hook.fn());},
                loop.executionPriority,
                loop.name + " - hook: " + hook.name);
            if (prev.definition()) h.then(prev);
            prev = h;
        }
        
        _taskManager.submit(prev);
    }
}