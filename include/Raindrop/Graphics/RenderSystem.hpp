#pragma once

#include "Raindrop/System/ISystem.hpp"
#include "GraphicsEngine.hpp"
#include <memory>

namespace Raindrop::Graphics{
    class RenderSystem : public Raindrop::System::ISystem{
        public:
            virtual ~RenderSystem() = default;

            virtual void initialize(Engine& engine) override;
            virtual void postInitialize() override;
            virtual void preShutdown() override;
            virtual void shutdown() override;

            virtual void preRender() override;
            virtual void render() override;
            virtual void postRender() override;

            virtual const char* name() const override;
            virtual std::vector<Dependency> dependencies() const override;

            template<typename T>
            inline std::shared_ptr<T> getRenderSystem(){
                return _engine->getSystemManager().getSystem<T>();
            }
        
        private:
            std::unique_ptr<GraphicsEngine> _engine;
    };
}