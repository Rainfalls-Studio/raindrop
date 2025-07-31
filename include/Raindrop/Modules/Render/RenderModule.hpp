#pragma once

#include "Raindrop/Core/Modules/IModule.hpp"

namespace Raindrop::Render{
    class RenderSystem : public Modules::IModule{
        public:
            virtual ~RenderSystem() = default;

            virtual Modules::Result initialize(Modules::InitHelper& init) override;
            virtual void shutdown() override;

            virtual std::string name() const noexcept override;
    };
}