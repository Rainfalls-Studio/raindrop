#include "Scheduler/Scheduler.hpp"
#include <Tasks/TaskManager.hpp>

#include <Raindrop/Time/Clock.hpp>
#include <Raindrop/Engine.hpp>
#include <Raindrop/Modules/InitHelper.hpp>
#include <spdlog/spdlog.h>


extern "C" RAINDROP_EXPORT Raindrop::IModule* CreateModule(){
	return new Scheduler::Scheduler();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::IModule* module){
	delete module;
}

namespace Scheduler{
    Tasks::TaskStatus StageResultToTaskStatus(const char* name, StageResult&& r){
        switch (r.type){
            case StageResult::SKIP: spdlog::warn("task \"{}\" skipped : {}", name,  r.getSkip().reason); [[fallthrough]];
            case StageResult::CONTINUE: return Tasks::TaskStatus::Completed();
            case StageResult::RETRY_HOOK: return Tasks::TaskStatus::Retry(r.getRetry().waitDuration);
        }
        return Tasks::TaskStatus::Failed("Unknown hook result !");
    }

    Scheduler::Scheduler(){}

    Scheduler::~Scheduler(){
    }

    Raindrop::Result Scheduler::initialize(Raindrop::InitHelper& helper){
        _engine = &helper.engine();
        _taskManager = helper.getDependencyAs<Tasks::TaskManager>("TaskManager");

        return Raindrop::Result::Success();
    }


    void Scheduler::shutdown(){
        spdlog::info("Shuting down loop scheduler...");
        for (auto [it, loop] : _loops){
            auto runtime = loop->runtime;

            for (auto& stage : loop->stages){
                stage->shutdown();
            }
            
            loop->stages.clear();
        }

        _loops.clear();
    }

    Loop Scheduler::createLoop(const std::string& name) {
        auto [it, inserted] = _loops.emplace(name, std::make_shared<LoopData>());

        LoopData& loop = *it->second;
        loop.name = name;
        loop.engine = _engine;

        return Loop(it->second);
    }

    void Scheduler::run(const Loop& loop){
        assert(loop);
        namespace Time = Raindrop::Time;


        auto taskManager = _taskManager.lock();
        if (!taskManager){
            spdlog::warn("The task manager is not valid");
        }
        
        auto data = loop.data();
        auto& runtime = data->runtime;

        // spdlog::trace("Running loop {}", data->name);

        runtime = std::make_shared<LoopData::Runtime>();
        runtime->loop = data.get();
        runtime->lastRunTime = Time::now();

        runtime->controller = taskManager->createTask(
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
                
                // auto delta = (now - loopRuntime->lastRunTime).as<Time::milliseconds>().count();
                
                // double d = static_cast<double>(delta);
                // double hz = 1000.0 / d;
                // spdlog::info("loop {} :: delta : {:.1f} ms or ~{:.1f} Hz", data->name, d, hz);
                
                loopRuntime->lastRunTime = now;
                submitLoopIteration(*data);
                return Tasks::TaskStatus::Completed();
            },
            data->executionPriority,
            loop.name() + " - controller"
        );

        taskManager->submit(runtime->controller);
    }

    Loop Scheduler::getLoop(const std::string& name){
        auto it = _loops.find(name);
        if (it != _loops.end()){
            return Loop(it->second);
        }
        return Loop();
    }

    void Scheduler::submitLoopIteration(LoopData& loop) {
        auto taskManager = _taskManager.lock();
        if (!taskManager){
            spdlog::warn("The task manager is not valid");
        }

        auto controller = loop.runtime->controller;

        Tasks::TaskHandle prev = controller;
        auto& stages = loop.stages;

        for (size_t i=stages.size(); i>0; i--) {
            auto& stage = stages[i-1];
            auto h = taskManager->createTask(
                [stage] -> Tasks::TaskStatus {return StageResultToTaskStatus(stage->name(),  stage->execute());},
                loop.executionPriority,
                loop.name + " - stage: " + stage->name());
            if (prev.definition()) h.then(prev);
            prev = h;
        }
        
        taskManager->submit(prev);
    }
}