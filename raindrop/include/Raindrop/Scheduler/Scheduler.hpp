#pragma once

#include "LoopData.hpp"
#include "Loop.hpp"
#include "Raindrop/Tasks/TaskManager.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Scheduler{
    class Scheduler {
        public:
            Scheduler(Engine& engine);
            ~Scheduler();

            Loop createLoop(const std::string& name);
            Loop getLoop(const std::string& name);
            void run(const Loop& loop);

            void shutdown();

        private:
            Engine& _engine;
            Tasks::TaskManager& _taskManager;
            
            std::unordered_map<std::string, std::shared_ptr<LoopData>> _loops;
            void submitLoopIteration(LoopData& loop);
    };
}