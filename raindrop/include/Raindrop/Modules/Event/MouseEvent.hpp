#pragma once

#include "Event.hpp"
#include <glm/glm.hpp>

namespace Raindrop::Event{
    enum class MouseButton{
        LEFT,
        RIGHT,
        MIDDLE,
        X1,
        X2
    };

    // empty struct for cathegorization
    class MouseEvent : public Event{};

    class MouseMovedEvent : public MouseEvent{
        public:
            MouseMovedEvent(
                const glm::vec2& position,
                const glm::vec2& relative
            ) : 
                _position{position},
                _relative{relative}
            {}

            inline const glm::vec2& position() const noexcept {
                return _position;
            }

            inline const glm::vec2& relative() const noexcept {
                return _relative;
            }
        
        private:
            glm::vec2 _position;
            glm::vec2 _relative;
    };

    class MouseButtonEvent : public MouseEvent{
        public:
            MouseButtonEvent(
                MouseButton button,
                bool down,
                uint8_t clicks,
                glm::vec2 position
            ) : 
                _button{button},
                _down{down},
                _clicks{clicks},
                _position{position}
            {}

            inline MouseButton button() const noexcept{
                return _button;
            }

            inline bool down() const noexcept{
                return _down;
            }

            inline uint8_t clicks() const noexcept{
                return _clicks;
            }

            inline const glm::vec2& position() const noexcept{
                return _position;
            }

    
        private:
            MouseButton _button;
            bool _down;
            uint8_t _clicks;
            glm::vec2 _position;
    };

    class MouseWheelEvent : public MouseEvent{
        public:
            MouseWheelEvent(
                glm::vec2 wheel,
                glm::vec2 position
            ) : 
                _wheel{wheel},
                _position{position}
            {}

            const glm::vec2& wheel() const noexcept{
                return _wheel;
            }

            const glm::vec2& position() const noexcept{
                return _position;
            }


        private:
            glm::vec2 _wheel;
            glm::vec2 _position;
    };
}