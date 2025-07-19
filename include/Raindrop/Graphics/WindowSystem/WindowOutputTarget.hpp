#pragma once

#include "Raindrop/Graphics/Output/IOutputTarget.hpp"
#include <memory>

namespace Raindrop::Graphics::WindowSystem{
    class WindowContext;
    class WindowOutputTarget : public Output::IOutputTarget{
        public:
            WindowOutputTarget(std::weak_ptr<WindowContext> context);
            virtual ~WindowOutputTarget() = default;

            virtual Definition getDefinition() const override;
            virtual const char* name() const override;
        
        private:
            std::weak_ptr<WindowContext> _context;
    };
}