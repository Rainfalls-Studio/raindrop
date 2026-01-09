#pragma once

#include <Raindrop/Modules/IModule.hpp>

namespace CycleManager{
    class Manager : public Raindrop::IModule{
        public:
            Manager();
            ~Manager();

            virtual Raindrop::Result initialize(Raindrop::InitHelper& helper) override;
            virtual void shutdown() override;

        private:
    };
}