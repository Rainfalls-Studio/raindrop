#include "Raindrop/Graphics/WindowSystem/WindowOutputTarget.hpp"
#include "Raindrop/Graphics/WindowSystem/WindowContext.hpp"
#include <stdexcept>

namespace Raindrop::Graphics::WindowSystem{
    WindowOutputTarget::WindowOutputTarget(std::weak_ptr<WindowContext> context) : _context{context}{}

    WindowOutputTarget::Definition WindowOutputTarget::getDefinition() const{
        auto lock = _context.lock();
        if (!lock){
            throw std::runtime_error("Window context is not valid");
        }

        auto window = lock->window.lock();
        if (!window){
            throw std::runtime_error("Window is not valid");
        }

        return Definition{
            .name = std::string(window->getTitle()),
            .bufferCount = static_cast<uint32_t>(lock->images.size())
        };
    }

    const char* WindowOutputTarget::name() const{
        return "Window output target";
    }
}