#include "Raindrop/Event/Manager.hpp"
#include "Raindrop/Input/Manager.hpp"
#include "Raindrop/Input/Storage.hpp"
#include "Raindrop/Window/Window.hpp"
#include "Raindrop/Window/WindowEvents.hpp"
#include "glm/fwd.hpp"
#include <Raindrop/Raindrop.hpp>
#include <iostream>

class WindowStorage : public Raindrop::Input::StorageGroup{
    public:
        WindowStorage(){
            definition.declare<glm::uvec2>("size", [this]() -> glm::uvec2& {return size;});
            definition.declare<glm::uvec2>("position", [this]() -> glm::uvec2& {return position;});
        }

        glm::uvec2 size;
        glm::uvec2 position;
};

class Testbed{
    public:
        Testbed(){
            _engine = std::make_shared<Raindrop::Engine>();
            _events = std::make_shared<Raindrop::Event::Manager>(*_engine);
            _inputs = std::make_shared<Raindrop::Input::Manager>(*_engine, _events);
            
            try {
                _window = std::make_unique<Raindrop::Window::Window>(*_engine, _events);
            } catch (const std::exception& e) {
                std::cerr << "Exception during window creation: " << e.what() << std::endl;
            }

            _inputs->declareStorage<WindowStorage>("window");

            _inputs->onEvent<Raindrop::Window::WindowResized>(
                [](Raindrop::Input::Storage& s, const Raindrop::Window::WindowResized& event){
                    s.getGroup<WindowStorage>("window").size = event.getSize();
            });

            _inputs->onEvent<Raindrop::Window::WindowMoved>(
                [](Raindrop::Input::Storage& s, const Raindrop::Window::WindowMoved& event){
                    s.getGroup<WindowStorage>("window").position = event.getPosition();
            });
        }

        void run(){
            bool running = true;

            _events->subscribe<Raindrop::Window::WindowCloseRequest>(
                [&running](auto) -> bool {
                    running = false;
                    return false;
                }
            );


            while (running){
                events();

                std::cout << _inputs->access<glm::uvec2>("window.position").x << std::endl;
            }
        }

    private:
        std::shared_ptr<Raindrop::Engine> _engine;
        std::shared_ptr<Raindrop::Event::Manager> _events;
        std::shared_ptr<Raindrop::Input::Manager> _inputs;
        std::unique_ptr<Raindrop::Window::Window> _window;

        void events(){
            _window->events();
        }

};

int main(){
    Testbed testbed;
    testbed.run();

    return 0;
}