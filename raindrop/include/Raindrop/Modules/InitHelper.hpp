#pragma once

#include "Raindrop/Engine.hpp"
#include "IModule.hpp"

namespace Raindrop{
    class InitHelper{
        public:
            inline InitHelper(Engine& engine, const std::unordered_map<Name, std::shared_ptr<IModule>>& dependencies, const std::shared_ptr<spdlog::logger>& logger) : _engine(engine), _dependencies(dependencies), _logger{logger} {}

            inline Engine& engine() const noexcept{
                return _engine;
            }

            inline ModuleManager& modules() const noexcept{
                return _engine.getModuleManager();
            }

            inline const std::unordered_map<Name, std::shared_ptr<IModule>>& dependencies() const noexcept{
                return _dependencies;
            }

            std::shared_ptr<IModule> getDependency(const Name& name) const{
                return _dependencies.at(name);
            }

            template<typename... Modules>
            auto getDependencies(const Modules&... modules){
                static_assert((std::is_same_v<Modules, const Name&> && ...), "The Modules names must be strings");
                return std::make_tuple((getDependency(modules))...);
            }

            template<typename T>
            std::shared_ptr<T> getDependencyAs(const Name& name){
                return std::dynamic_pointer_cast<T>(getDependency(name));
            }

            template<typename... Ts, typename... Modules>
            auto getDependenciesAs(Modules&&... modules){
                static_assert(sizeof...(Ts) == sizeof...(Modules), "Types and names must match in count");
                static_assert((std::is_constructible<Name, Modules>::value && ...), "The Modules names must be strings");
                return std::make_tuple(getDependencyAs<Ts>(Name(modules))...);
            }

            std::shared_ptr<spdlog::logger> logger() const noexcept{
                return _logger;
            }

        private:
            Engine& _engine;
            std::unordered_map<Name, std::shared_ptr<IModule>> _dependencies;
            std::shared_ptr<spdlog::logger> _logger;

    };
}