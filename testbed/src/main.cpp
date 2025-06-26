#include "Raindrop/Asset/Manager.hpp"
#include "Raindrop/Event/Manager.hpp"
#include "Raindrop/Graphics/Config.hpp"
#include "Raindrop/Graphics/Engine.hpp"
#include "Raindrop/Input/Manager.hpp"
#include "Raindrop/Input/Storage.hpp"
#include "Raindrop/Layer/Manager.hpp"
#include "Raindrop/Window/Config.hpp"
#include "Raindrop/Window/Manager.hpp"
#include "Raindrop/Window/Position.hpp"
#include "Raindrop/Window/Window.hpp"
#include "Raindrop/Window/WindowEvents.hpp"
#include "glm/fwd.hpp"
#include <Raindrop/Raindrop.hpp>
#include <exception>
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
            
            createAssetManager();
            createEventManager();
            createInputManager();
            createWindowManager();
            createWindow();
            createLayerManager();
            createGraphicsEngine();

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
                update();
                render();
            }
        }

    private:
        std::shared_ptr<Raindrop::Engine> _engine;
        std::shared_ptr<Raindrop::Asset::Manager> _assets;
        std::shared_ptr<Raindrop::Event::Manager> _events;
        std::shared_ptr<Raindrop::Input::Manager> _inputs;
        std::shared_ptr<Raindrop::Window::Manager> _windows;
        std::shared_ptr<Raindrop::Graphics::Engine> _graphics;
        std::shared_ptr<Raindrop::Layer::Manager> _layers;

        std::shared_ptr<Raindrop::Window::Window> _window;

        void createAssetManager(){
            _assets = std::make_shared<Raindrop::Asset::Manager>(*_engine);
        }

        void createEventManager(){
            _events = std::make_shared<Raindrop::Event::Manager>(*_engine);
        }

        void createInputManager(){
            _inputs = std::make_shared<Raindrop::Input::Manager>(*_engine, _events);
        }

        void createWindowManager(){
            _windows = std::make_shared<Raindrop::Window::Manager>(*_engine);
        }

        void createLayerManager(){
            _layers = std::make_shared<Raindrop::Layer::Manager>(*_engine);
        }

        void createGraphicsEngine(){
            Raindrop::Graphics::Config config(*_engine);

            config
                .setAssetManager(_assets)
                .setLayerManager(_layers)
                .setWindowManager(_windows);

            _graphics = std::make_shared<Raindrop::Graphics::Engine>(config);
        }

        void createWindow(){
            auto config = Raindrop::Window::Config::Default(*_engine);

            config
                .setPosition({Raindrop::Window::POSITION_CENTRED, Raindrop::Window::POSITION_CENTRED})
                .setEventManager(_events);

            _window = _windows->createWindow(config);
        }

        void events(){
            _windows->foreach(
                [](Raindrop::Window::Window& window){
                    window.events();
                }
            );
        }

        void update(){

        }

        void render(){
            // _windows->foreach([this](Raindrop::Window::Window& window){
            //     _graphics->render(window);
            // })
        }
};

int main(){

    try {
        Testbed testbed;
        testbed.run();
    } catch (const std::exception &e){
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}