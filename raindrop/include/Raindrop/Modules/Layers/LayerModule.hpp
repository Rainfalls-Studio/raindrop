#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"
#include "Raindrop/Engine.hpp"
#include "LayerManager.hpp"

namespace Raindrop::Layers{
    class LayerModule : public Modules::IModule{
        public:
            LayerModule();
            virtual ~LayerModule() override;

            virtual Modules::Result initialize(Modules::InitHelper& helper) override;
            virtual void shutdown() override;
 
            virtual Name name() const noexcept override;
            virtual Modules::DependencyList dependencies() const noexcept override;

            LayerManager* manager();
            const LayerManager* manager() const;

        private:
            Engine* _engine;
            std::unique_ptr<LayerManager> _manager;

            void createLogger();
    };
}