#pragma once

#include <string>
#include <memory>
#include "ISystem.hpp"
#include "SystemGraph.hpp"

namespace Raindrop::Systems{
    class Manager{
        public:
            Manager(Engine& engine);

            std::expected<void, Error> registerSystem(const std::shared_ptr<ISystem>& system);

            template<typename T>
            void registerSystem(){
                registerSystem(std::make_shared<T>());
            }

            template<typename T, typename... Args>
            void registerSystem(Args&&... args){
                registerSystem(std::make_shared<T>(std::forward<Args>(args)...));
            }

            std::shared_ptr<ISystem> getSystem(const std::string& name) noexcept;

            template<typename... Systems>
            auto getSystems(const Systems&... systems){
                static_assert((std::is_same_v<Systems, const std::string&> && ...), "The systems names must be strings");
                return std::make_tuple((getSystem(systems))...);
            }

            template<typename T>
            std::shared_ptr<T> getSystemAs(const std::string& name){
                return std::dynamic_pointer_cast<T>(getSystem(name));
            }

            template<typename... Ts, typename... Systems>
            auto getSystemsAs(Systems&&... systems){
                static_assert(sizeof...(Ts) == sizeof...(Systems), "Types and names must match in count");
                static_assert((std::is_constructible<std::string, Systems>::value && ...), "The systems names must be strings");
                return std::make_tuple(getSystemAs<Ts>(std::string(systems))...);
            }

            std::expected<void, Error> initialize();
            void shutdown();

            
        private:
            Engine& _engine;
            bool _initialized;
            SystemGraph _graph;
    };
}