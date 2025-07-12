#pragma once

#include "Raindrop/System/ISystem.hpp"
#include "Manager.hpp"

namespace Raindrop::Event{
    class EventSystem : public System::ISystem{
        public:
            EventSystem();
            virtual ~EventSystem() = default;

            virtual void initialize(Engine& engine) override;
            virtual void shutdown() override;

            virtual void event() override;

            virtual const char* name() const override;


            Manager& getManager() noexcept;
        
        private:
            std::unique_ptr<Manager> _manager;
    };
}