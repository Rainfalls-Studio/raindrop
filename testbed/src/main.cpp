#include "Raindrop/Event/Manager.hpp"
#include "Raindrop/Window/Window.hpp"
#include <Raindrop/Raindrop.hpp>
#include <iostream>

class Testbed{
    public:
        Testbed(){
            _engine = std::make_shared<Raindrop::Engine>();
            _events = std::make_shared<Raindrop::Event::Manager>(*_engine);

            _window = std::make_unique<Raindrop::Window::Window>(*_engine, _events);
        }

        void run(){
            bool running = false;

            while (running){
                events();
            }
        }

    private:
        std::shared_ptr<Raindrop::Engine> _engine;
        std::shared_ptr<Raindrop::Event::Manager> _events;
        std::unique_ptr<Raindrop::Window::Window> _window;

        void events(){

        }

};

int main(){
    Testbed testbed;

    return 0;
}

// int main(){
//     auto engine = Raindrop::Engine::Create();

//     // Create base subsystems
//     auto event = Raindrop::Event::Manager::Create(engine);
//     auto input = Raindrop::Input::Manager::Create(engine);
//     auto asset = Raindrop::Asset::Manager::Create(engine);
//     auto config = Raindrop::Config::Tree::Create(engine);
//     auto scenes = Raindrop::Scene::Manager::Create(engine);

//     // Create a new window and attach the subsystems.
//     auto window = Raindrop::Window::Create();
//     window.attach(
//         event,
//         input,
//         config["window"]
//     );
    
//     // Subscribe to the close window event
//     bool running = true;
//     event.subscribte<Raindrop::Event::Window::OnClose>(
//         [](const Raindrop::Event::Window::OnClose& event){
//             running = false;
//         }
//     );

//     // Create a graphics engine
//     auto graphicsEngine = Raindrop::Graphics::Engine::Create(engine);
//     graphicsEngine.attach(
//         event,
//         config["graphics"],
//         asset,
//         window // it is possible for the window to close and reopen
//     );

//     auto layerSystem = Raindrop::Layer::Manager::Create(engine);
//     layerSystem.attach(
//         event // Events will be propagated from top to bottom
//     );

//     #ifdef RAINDROP_DEV_MODE
//         auto debug = layerSystem.addLayer(Raindrop::Layer::OVERLAY, "debug");
//         debug.attach(
//             graphicsEngine.getRenderer("ImGUI"),
//             asset,
//             config["debug"]
//         );
//     #endif

//     auto gameplay = layerSystem.addLayer(Raindrop::Layer::CONTENT, "gameplay");
//     gameplay.attach(
//         graphicsEngine.getRenderer("Prograde"), // The engine's renderer
//         asset,
//         config["graphics"]
//     );


//     while (running){
//         input.poll();

//         scenes.foreach([](const Raindrop::Scene::Scene& scene){

//         });
//     }

//     return 0;
// }