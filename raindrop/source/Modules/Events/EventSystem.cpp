#include "Raindrop/Modules/Event/EventModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

namespace Raindrop::Event{
    EventModule::EventModule(){}

    Modules::Result EventModule::initialize(Modules::InitHelper& init){

        _manager = std::make_unique<Manager>(init.engine());

        return Modules::Result::Success();
    }

    void EventModule::shutdown(){
        _manager.reset();
    }

    std::string EventModule::name() const noexcept{
        return "Event";
    }

    Manager& EventModule::getManager() noexcept{
        assert(_manager && "The manager must be valid");
        return *_manager;
    }
}