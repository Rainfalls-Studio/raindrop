#pragma once

#include <memory>

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Scheduler{
    class Loop;
    struct LoopData;
    struct Hook;

    class StageInitHelper{
        public:
            StageInitHelper(Engine& engine, const std::shared_ptr<LoopData>& data);

            Engine& engine();
            Loop loop();
            void registerHook(const Hook& hook);

        private:
            Engine& _engine;
            const std::shared_ptr<LoopData>& _loopData;
    };
}