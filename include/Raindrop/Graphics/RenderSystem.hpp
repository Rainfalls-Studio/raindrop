#pragma once

#include "Raindrop/System/ISystem.hpp"

namespace Raindrop::Graphics{
    class RenderSystem : public System::ISystem{
        public:
            RenderSystem();

            virtual void initialize(Engine& engine) override;

            virtual const char* name() const override;
            virtual std::vector<Dependency> dependencies() const override;
    };
}