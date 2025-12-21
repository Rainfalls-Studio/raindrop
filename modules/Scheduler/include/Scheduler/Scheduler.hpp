#pragma once

#include "LoopData.hpp"
#include "Loop.hpp"
#include <Tasks/TaskManager.hpp>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Scheduler{
    class Scheduler : public Modules::IModule{
        public:
            Scheduler();
            virtual ~Scheduler() override;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            inline virtual void shutdown() override;

            Loop createLoop(const std::string& name);
            Loop getLoop(const std::string& name);
            void run(const Loop& loop);


        private:
            Engine* _engine;
            std::weak_ptr<Tasks::TaskManager> _taskManager;
            
            std::unordered_map<std::string, std::shared_ptr<LoopData>> _loops;
            void submitLoopIteration(LoopData& loop);
    };
}