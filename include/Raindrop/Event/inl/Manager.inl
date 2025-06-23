#pragma once

#include "../Manager.hpp"
#include "Raindrop/Core/Internal/Event/Manager.hpp"

namespace Raindrop::Core::Event{
    inline Manager::Manager() : _impl{}{}
    inline Manager::Manager(std::shared_ptr<Core::Internal::Event::Manager> impl) : _impl{impl}{}

    inline bool Manager::isValid() const noexcept{
        return _impl != nullptr;
    }

    inline void Manager::subscribe(const std::type_index& id, const CallbackInfo& info){
        assertGet().subscribe(id, info);
    }

    inline void Manager::trigger(const std::type_index& id, const Event& event){
        assertGet().trigger(id, event);
    }

    template<typename EventType>
    inline void Manager::subscribe(std::function<bool(const EventType&)> callback){
        assertGet().subscribe<EventType>(callback);
    }

    template<typename EventType, typename ClassType>
    inline void Manager::subscribe(ClassType& instance, bool (ClassType::*memberFunction)(const EventType&)){
        assertGet().subscribe<EventType, ClassType>(instance, memberFunction);
    }

    template<typename EventType, typename ClassType>
    inline void Manager::subscribe(ClassType* instance, bool (ClassType::*memberFunction)(const EventType&)){
        assertGet().subscribe<EventType, ClassType>(instance, memberFunction);
    }

    template<typename T>
    inline void Manager::trigger(const T& event){
        assertGet().trigger<T>(event);
    }

    inline Core::Internal::Event::Manager* Manager::get(){
        return _impl.get();
    }

    inline Core::Internal::Event::Manager& Manager::assertGet(){
        Core::Internal::Event::Manager* ptr = get();

        assert(ptr != nullptr && "The event manager interface is invalid");

        return *ptr;
    }
}