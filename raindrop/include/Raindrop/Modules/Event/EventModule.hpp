#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Manager.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Event{
    class EventModule : public Modules::IModule{
        public:
            EventModule();
            virtual ~EventModule() = default;

            virtual Modules::Result initialize(Modules::InitHelper& init) override;
            virtual void shutdown() override;

            virtual std::string name() const noexcept override;

            Manager& getManager() noexcept;
        
        private:
            std::unique_ptr<Manager> _manager;
    };
}