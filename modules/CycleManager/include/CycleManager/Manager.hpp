#pragma once

#include <Raindrop/Modules/IModule.hpp>

namespace CycleManager{
    class Manager : public Raindrop::Modules::IModule{
        public:
            Manager();
            ~Manager();

            virtual Raindrop::Modules::Result initialize(Raindrop::Modules::InitHelper& helper) override;
            virtual void shutdown() override;

        private:
    };
}