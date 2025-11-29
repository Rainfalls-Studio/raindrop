#pragma once

#include "Event.hpp"
#include "Key.hpp"
#include <glm/glm.hpp>
#include <string>

namespace Raindrop::Event{
    
    class KeyboardEvent : public Event{
        // KeyboardID keyboard;
    };

    class TextInputEvent : public KeyboardEvent{
        public:
            TextInputEvent(const std::string& text) : _text{text}{}

            inline const std::string& text() const noexcept{
                return _text;
            }

        private:
            std::string _text;
    };

    struct KeyEvent : public KeyboardEvent{
        public:
            KeyEvent(
                Key key,
                PhysicalKey physicalKey,
                bool down,
                bool repeat
            ) : 
                _key{key},
                _physicalKey{physicalKey},
                _down{down},
                _repeat{repeat}
            {}

            inline Key key() const noexcept{
                return _key;
            }

            inline PhysicalKey physicalKey() const noexcept{
                return _physicalKey;
            }

            inline bool down() const noexcept{
                return _down;
            }

            inline bool repeate() const noexcept{
                return _repeat;
            }

        private:
            Key _key;
            PhysicalKey _physicalKey;
            bool _down;
            bool _repeat;
    };
}