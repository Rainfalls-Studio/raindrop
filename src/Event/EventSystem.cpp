#include "Raindrop/Event/EventSystem.hpp"

namespace Raindrop::Event{
    EventSystem::EventSystem(){}

    void EventSystem::initialize(Engine& engine){
        _manager = std::make_unique<Manager>(engine);
    }

    void EventSystem::shutdown(){
        _manager.reset();
    }

    void EventSystem::event(){}

    const char* EventSystem::name() const{
        return "Event manager";
    }

    Manager& EventSystem::getManager() noexcept{
        assert(_manager && "The manager must be valid");
        return *_manager;
    }
}