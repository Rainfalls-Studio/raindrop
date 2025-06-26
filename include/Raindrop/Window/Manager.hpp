#pragma once

#include "Raindrop/Event/Manager.hpp"
#include "Raindrop/Window/Window.hpp"
#include <cstddef>
#include <functional>

namespace Raindrop::Window{
    class Manager{
        public:
            Manager(Engine& engine);

            std::shared_ptr<Window> createWindow(const Config& config);
            void foreach(std::function<void(Window&)> callback);

            std::size_t getSize() const;

            const std::list<std::weak_ptr<Window>>& getWindows();
            

        private:
            Engine& _engine;
            std::list<std::weak_ptr<Window>> _windows;
    };
}