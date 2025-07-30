#pragma once

#include <vector>
#include <expected>
#include <memory>

#include "../Error.hpp"
#include "Dependency.hpp"
#include "IModule.hpp"

namespace Raindrop{
    class Engine;
}

namespace Raindrop::Systems{
    class ISystem;

    class ISystem{
        friend class Manager;
        public:
            virtual ~ISystem() = default;

            virtual std::expected<void, Error> initialize(Engine& engine) = 0;
            inline virtual void postInitialize() {}
            inline virtual void preShutdown() {}
            inline virtual void shutdown() {}

            virtual std::string name() const noexcept = 0;
            inline virtual std::vector<Dependency> dependencies() const noexcept {return {};}

            inline virtual void registerModule(const std::shared_ptr<IModule>) {}
            inline virtual void unregisterModule(const std::string&) {}
            inline virtual std::shared_ptr<IModule> getModule(const std::string&) {return {};}
            inline virtual std::vector<std::string> listModules() {return {};}

            template<typename... Modules>
            auto getModules(const Modules&... modules){
                static_assert((std::is_same_v<Modules, const std::string&> && ...), "The module names must be strings");
                return std::make_tuple((getModule(modules))...);
            }

            template<typename T>
            std::shared_ptr<T> getModuleAs(const std::string& name){
                return std::dynamic_pointer_cast<T>(getModule(name));
            }

            template<typename... Ts, typename... Modules>
            auto getModulesAs(const Modules&... modules){
                static_assert(sizeof...(Ts) == sizeof...(Modules), "Types and names must match in count");
                static_assert((std::is_same_v<Modules, const std::string&> && ...), "The module names must be strings");
                return std::make_tuple(getModuleAs<Ts>(modules)...);
            }
    };
}