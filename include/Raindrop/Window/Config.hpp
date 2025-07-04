#pragma once

#include <string>
#include "Raindrop/Event/Manager.hpp"
#include "Flags.hpp"
#include "Position.hpp"
#include "Size.hpp"

namespace Raindrop::Window{
    class Config{
        public:
            static Config Empty(Engine& engine);
            static Config Tooltip(Engine& engine);
            static Config Default(Engine& engine);

            Config(Engine& engine);

            Config& setPosition(const Position& position);
            Config& setSize(const Size& size);
            Config& setFlags(const Flags& flags);
            Config& setTitle(const std::string& title);
            Config& setEventManager(const std::shared_ptr<Event::Manager>& manager);

            Engine& _engine;
            std::shared_ptr<Event::Manager> _events;

            Position _position;
            Size _size;
            Flags _flags;
            std::string _title;
    };
}