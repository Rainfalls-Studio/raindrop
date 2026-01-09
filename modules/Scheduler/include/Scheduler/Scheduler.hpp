#pragma once

#include "LoopData.hpp"
#include "Loop.hpp"
#include <Tasks/TaskManager.hpp>

namespace Raindrop{
    class Engine;
}

namespace Scheduler{
    class Scheduler : public Raindrop::IModule{
        public:
            Scheduler();
            virtual ~Scheduler() override;

            virtual Raindrop::Result initialize(Raindrop::InitHelper& helper) override;
            inline virtual void shutdown() override;

            Loop createLoop(const std::string& name);
            Loop getLoop(const std::string& name);
            void run(const Loop& loop);


        private:
            Raindrop::Engine* _engine;
            std::weak_ptr<Tasks::TaskManager> _taskManager;
            
            std::unordered_map<std::string, std::shared_ptr<LoopData>> _loops;
            void submitLoopIteration(LoopData& loop);
    };
}