#pragma once

#include "Raindrop/Core/Systems/ISystem.hpp"

namespace Raindrop::Render{
    class RenderSystem : public Systems::ISystem{
        public:
            virtual ~RenderSystem() = default;

            virtual std::expected<void, Error> initialize(Engine& engine) override;
            virtual void shutdown() override;

            virtual std::string name() const noexcept override;
    };
}