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

            inline Modules::Manager& getModules() const noexcept{
                return _engine.getModuleManager();
            }

            inline const ModuleMap& dependencies() const noexcept{
                return _dependencies;
            }

        private:
            Engine& _engine;
            ModuleMap _dependencies;

    };
}