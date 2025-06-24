#pragma once

namespace Raindrop::Input{
    template<typename T>
    void Manager::declareStorage(const std::string& name){
        declareStorage(name, std::make_unique<T>());
    }

    template<typename T, typename... Args>
    void Manager::declareStorage(const std::string& name, Args... args){
        declareStorage(name, std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename T>
    void Manager::onEvent(std::function<void(Storage& storage, const T&)> callback){
        static_assert(std::is_base_of<Event::Event, T>::value, "T must be derived from Event::Event");
        if (!_events) return;

        _events->subscribe<T>([callback, this](const T& event) -> bool {
            callback(_storage, event);
            return false;
        });
    }


    template<typename T>
    T& Manager::access(const std::string& name){
        return _storage.access<T>(name);
    }

    template<typename T>
    T& Manager::getGroup(const std::string& name){
        return _storage.getGroup<T>(name);
    }
}