#pragma once

#include <memory>
#include "LayerRegistry.hpp"
#include "Layer.hpp"
#include "System.hpp"

namespace Raindrop{
    class Engine{
        public:
            Engine();

            static std::shared_ptr<Engine> Create();

            // =========== system ==============

            template<typename T, typename... Args>
            void addSystem(Args&&... args){
                static_assert(std::is_base_of<System, T>::value, "T must be derived from System");
                addSystem(std::make_shared<T>(std::forward<Args>(args)...));
            }

            template<typename T>
            void addSystem(){
                static_assert(std::is_base_of<System, T>::value, "T must be derived from System");
                addSystem(std::make_shared<T>());
            }

            void addSystem(std::shared_ptr<System> system);

            // =========== layers ==============

            LayerRegistry& getLayerRegistry() noexcept;

            inline Layer createLayer(){
                return getLayerRegistry().createLayer();
            }

        private:
            LayerRegistry _layers;
            std::list<std::shared_ptr<System>> _system;
    };
}