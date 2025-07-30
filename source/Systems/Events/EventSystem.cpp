#include "Raindrop/Systems/Event/EventSystem.hpp"

namespace Raindrop::Event{
    EventSystem::EventSystem(){}

    std::expected<void, Error> EventSystem::initialize(Engine& engine){
        _manager = std::make_unique<Manager>(engine);
        return{};
    }

    void EventSystem::shutdown(){
        _manager.reset();
    }

    std::string EventSystem::name() const noexcept{
        return "Event";
    }

    Manager& EventSystem::getManager() noexcept{
        assert(_manager && "The manager must be valid");
        return *_manager;
    }
}