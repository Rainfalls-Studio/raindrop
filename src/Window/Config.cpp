#include "Raindrop/Window/Config.hpp"
#include "Raindrop/Window/Position.hpp"

namespace Raindrop::Window{
    
    Config Config::Empty(Engine& engine){
        return Config(engine)
            .setPosition({POSITION_UNDEFINED, POSITION_UNDEFINED})
            .setSize({1, 1})
            .setTitle("")
            .setFlags(
                Flags::HIDDEN |
                Flags::NOT_FOCUSABLE
            );
    }

    Config Config::Tooltip(Engine& engine){
        return Config(engine)
            .setPosition({POSITION_UNDEFINED, POSITION_UNDEFINED})
            .setSize({200, 100})
            .setTitle("")
            .setFlags(
                Flags::BORDERLESS |
                Flags::ALWAYS_ON_TOP |
                Flags::NOT_FOCUSABLE |
                Flags::MOUSE_CAPTURE 
            );
    }

    Config Config::Default(Engine& engine){
        return Config(engine)
            .setPosition({POSITION_UNDEFINED, POSITION_UNDEFINED})
            .setSize({800, 600})
            .setTitle("Default")
            .setFlags(
                Flags::RESIZABLE
            );
    }

    Config::Config(Engine& engine) : _engine{engine}{}

    Config& Config::setPosition(const Position& position){
        _position = position;
        return *this;
    }

    Config& Config::setSize(const Size& size){
        _size = size;
        return *this;
    }

    Config& Config::setFlags(const Flags& flags){
        _flags = flags;
        return *this;
    }

    Config& Config::setTitle(const std::string& title){
        _title = title;
        return *this;
    }

    Config& Config::setEventManager(const std::shared_ptr<Event::Manager>& manager){
        _events = manager;
        return *this;
    }

}