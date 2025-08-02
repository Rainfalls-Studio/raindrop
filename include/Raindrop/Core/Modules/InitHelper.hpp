#pragma once

#include "Raindrop/Engine.hpp"
#include "IModule.hpp"

namespace Raindrop::Modules{
    class InitHelper{
        public:
            inline InitHelper(Engine& engine, const ModuleMap& dependencies) : _engine(engine), _dependencies(dependencies) {}

            inline Engine& engine() const noexcept{
                return _engine;
            }

            inline Scheduler::Scheduler& scheduler() const noexcept{
                return _engine.getScheduler();
            }

            inline Layers::Manager& layers() const noexcept{
                return _engine.getLayerManager();
            }

            inline Modules::Manager& modules() const noexcept{
                return _engine.getModuleManager();
            }

            inline const ModuleMap& dependencies() const noexcept{
                return _dependencies;
            }

            
            SharedModule getDependency(const IModule::Name& name) const{
                return _dependencies.at(name);
            }

            template<typename... Modules>
            auto getDependencies(const Modules&... modules){
                static_assert((std::is_same_v<Modules, const IModule::Name&> && ...), "The Modules names must be strings");
                return std::make_tuple((getDependency(modules))...);
            }

            template<typename T>
            std::shared_ptr<T> getDependencyAs(const IModule::Name& name){
                return std::dynamic_pointer_cast<T>(getDependency(name));
            }

            template<typename... Ts, typename... Modules>
            auto getDependenciesAs(Modules&&... modules){
                static_assert(sizeof...(Ts) == sizeof...(Modules), "Types and names must match in count");
                static_assert((std::is_constructible<IModule::Name, Modules>::value && ...), "The Modules names must be strings");
                return std::make_tuple(getDependencyAs<Ts>(IModule::Name(modules))...);
            }

        private:
            Engine& _engine;
            ModuleMap _dependencies;

    };
}