#pragma once

#include "../System.hpp"

namespace Raindrop::Systems{
    class Renderer : public System{
        public:
            Renderer();
            virtual ~Renderer() override;

            virtual void onFrame() override;
        
        private:
            // Raindrop::Graphics::Renderer
    };
}