#pragma once

#include "Raindrop/System/ISystem.hpp"
#include "Raindrop/Layer/Manager.hpp"
#include "LayerModule.hpp"

namespace Raindrop::Scene{
    class SceneSystem : public System::ISystem{
        public:
            virtual ~SceneSystem() = default;

            virtual void initialize(Engine& engine) override;
            virtual void postInitialize() override;

            virtual void preShutdown() override;
            virtual void shutdown() override;

            virtual void top() override;
            virtual void bottom() override;

            virtual void preUpdate() override;
            virtual void update() override;
            virtual void postUpdate() override;

            virtual void preRender() override;
            virtual void render() override;
            virtual void postRender() override;
            
            virtual const char* name() const override;
        
        private:
            Engine* _engine;
            decltype(std::declval<Layer::Manager>().view<LayerModule>()) _layers;

            void foreachLayer(const std::function<void(Scene&)>& func);
    };
}