#include "Raindrop/Components/Scene.hpp"
#include "Raindrop/Systems/Scene.hpp"
#include <Raindrop/Raindrop.hpp>
#include <iostream>

// class WindowStorage : public Raindrop::Input::StorageGroup{
//     public:
//         WindowStorage(){
//             definition.declare<glm::uvec2>("size", [this]() -> glm::uvec2& {return size;});
//             definition.declare<glm::uvec2>("position", [this]() -> glm::uvec2& {return position;});
//         }

//         glm::uvec2 size;
//         glm::uvec2 position;
// };

class Testbed{
    public:
        Testbed(){

            _engine = std::make_shared<Raindrop::Engine>();
            _engine->addSystem<Raindrop::Systems::Scene>();

            Raindrop::Layer layer = _engine->createLayer();
            layer.add<Raindrop::Components::Scene>();
            

            // createAssetManager();
            // createEventManager();
            // createInputManager();
            // createWindowManager();
            // createWindow();
            // createSceneManager();

            // _inputs->declareStorage<WindowStorage>("window");

            // _inputs->onEvent<Raindrop::Window::WindowResized>(
            //     [](Raindrop::Input::Storage& s, const Raindrop::Window::WindowResized& event){
            //         s.getGroup<WindowStorage>("window").size = event.getSize();
            // });

            // _inputs->onEvent<Raindrop::Window::WindowMoved>(
            //     [](Raindrop::Input::Storage& s, const Raindrop::Window::WindowMoved& event){
            //         s.getGroup<WindowStorage>("window").position = event.getPosition();
            // });
        }

        // void run(){
        //     bool running = true;

        //     _events->subscribe<Raindrop::Window::WindowCloseRequest>(
        //         [&running](auto) -> bool {
        //             running = false;
        //             return false;
        //         }
        //     );


        //     while (running){
        //         events();
        //         update();
        //         render();
        //     }
        // }

    private:
        std::shared_ptr<Raindrop::Engine> _engine;
        // std::shared_ptr<Raindrop::Asset::Manager> _assets;
        // std::shared_ptr<Raindrop::Event::Manager> _events;
        // std::shared_ptr<Raindrop::Input::Manager> _inputs;
        // std::shared_ptr<Raindrop::Window::Manager> _windows;
        // std::shared_ptr<Raindrop::Scene::Manager> _scenes;

        // std::shared_ptr<Raindrop::Window::Window> _window;

        // std::shared_ptr<Raindrop::Scene::Scene> _scene;

        // void createAssetManager(){
        //     _assets = std::make_shared<Raindrop::Asset::Manager>(*_engine);
        // }

        // void createEventManager(){
        //     _events = std::make_shared<Raindrop::Event::Manager>(*_engine);
        // }

        // void createInputManager(){
        //     _inputs = std::make_shared<Raindrop::Input::Manager>(*_engine, _events);
        // }

        // void createWindowManager(){
        //     _windows = std::make_shared<Raindrop::Window::Manager>(*_engine);
        // }


        // void createSceneManager(){
        //     _scenes = std::make_shared<Raindrop::Scene::Manager>(*_engine);

        // }

        // void createWindow(){
        //     auto config = Raindrop::Window::Config::Default(*_engine);

        //     config
        //         .setPosition({Raindrop::Window::POSITION_CENTRED, Raindrop::Window::POSITION_CENTRED})
        //         .setEventManager(_events);

        //     _window = _windows->createWindow(config);
        // }

        // void events(){
        //     _windows->foreach(
        //         [](Raindrop::Window::Window& window){
        //             window.events();
        //         }
        //     );
        // }

        // void update(){
            
        // }

        // void render(){
        //     // _windows->foreach([this](Raindrop::Window::Window& window){
        //     //     _graphics->render(window);
        //     // })
        // }
};

int main(){

    try {
        Testbed testbed;
        // testbed.run();
    } catch (const std::exception &e){
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}