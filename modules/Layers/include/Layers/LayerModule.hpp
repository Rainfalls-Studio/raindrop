#pragma once

#include <Raindrop/Modules/IModule.hpp>
#include <Raindrop/Engine.hpp>

#include "LayerManager.hpp"

namespace Layers{
    class LayerModule : public Raindrop::IModule{
        public:
            LayerModule();
            virtual ~LayerModule() override;

            virtual Raindrop::Result initialize(Raindrop::InitHelper& helper) override;
            virtual void shutdown() override;
            
            LayerManager* manager();
            const LayerManager* manager() const;

        private:
            Raindrop::Engine* _engine;
            std::unique_ptr<LayerManager> _manager;

            void createLogger();
    };
}