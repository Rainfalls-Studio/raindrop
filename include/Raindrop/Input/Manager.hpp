#pragma once

#include "../Engine.hpp"
#include "../Event/Manager.hpp"
#include "Storage.hpp"
#include <type_traits>

namespace Raindrop::Input{
    class Manager{
        public:
            Manager(Engine& engine, std::shared_ptr<Event::Manager> events = nullptr);

            template<typename T>
            void declareStorage(const std::string& name);

            template<typename T, typename... Args>
            void declareStorage(const std::string& name, Args... args);

            void declareStorage(const std::string& name, std::unique_ptr<StorageGroup> group);

            template<typename T>
            void onEvent(std::function<void(Storage& storage, const T&)> callback);

            void* access(const std::string& name);

            template<typename T>
            T& access(const std::string& name);


            StorageGroup& getGroup(const std::string& name);

            template<typename T>
            T& getGroup(const std::string& name);

        private:
            Engine& _engine;
            std::shared_ptr<Event::Manager> _events;
            Storage _storage;
    };
}

#include "inl/Manager.inl"