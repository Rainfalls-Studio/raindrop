#include <Raindrop/Raindrop.hpp>


class Testbed : public Raindrop::Systems::ISystem{
    public:

        Testbed(){}
        virtual ~Testbed() override = default;

        virtual std::expected<void, Raindrop::Error> initialize(Raindrop::Engine& engine) override{
            _engine = &engine;
            auto& systems = engine.getSystemManager();

            systems.getSystemAs<Raindrop::Event::EventSystem>("Event")->getManager().subscribe<Raindrop::Window::Events::WindowCloseRequest>(
                [this](const Raindrop::Window::Events::WindowCloseRequest& event) -> bool {
                    if (event.getWindow() == _window){
                        _engine->stop();
                    }
                    return false;
                }
            );

            createWindow();
            createGameplayLayer();
            createDebugLayer();

            auto& scheduler = engine.getScheduler();

            updateSubscription = scheduler.subscribe([this]{update();}, Raindrop::Scheduler::Priority::UPDATE);

            return {};
        }

        virtual std::vector<Raindrop::Systems::Dependency> dependencies() const noexcept override{
            using Raindrop::Systems::Dependency;

            return {
                Dependency("Event"),
                // Dependency("Scene"),
                // Dependency("Render"),
                Dependency("Window")
            };
        }

        virtual std::string name() const noexcept override{
            return "Testbed";
        }

        void createWindow(){
            // Get systems
            auto& systems = _engine->getSystemManager();

            // auto [windowSys, renderSys] = systems.getSystemsAs<
            //     Raindrop::Window::WindowSystem,
            //     Raindrop::Render::RenderSystem>(
            //         "Window",
            //         "Render"
            //     );

            auto windowSys = systems.getSystemAs<Raindrop::Window::WindowSystem>("Window");

            using Raindrop::Window::WindowFlags;

            // Create a window
            Raindrop::Window::WindowConfig config {
                .resolution = {800, 600},
                .position = {0, 0},
                .title = "testbed",
                .flags = {
                    WindowFlags::RESIZABLE
                }
            };
            _window = windowSys->createWindow(config);

            // Register it in the graphics window system
            // auto windowRenderSys = renderSys->getRenderSystem<Raindrop::Graphics::WindowSystem::WindowSystem>();
            // windowRenderSys->registerWindow(_window);
        }

        void createGameplayLayer(){
            // auto& layers = _engine->getLayerManager();
            
            // Raindrop::Layers::Layer gameplay = layers.createLayer();
            // auto& scene = gameplay.emplaceModule<Raindrop::Scene::SceneModule>().scene;

            // scene.emplaceBehavior<Raindrop::Behaviors::TagAttacherBehavior>();
            // scene.emplaceBehavior<Raindrop::Behaviors::TransformAttacherBehavior>();
            // scene.emplaceBehavior<Raindrop::Behaviors::HierarchyAttacherBehavior>();
            // scene.emplaceBehavior<Raindrop::Behaviors::HierarchyTransformPropagator>();

            // Raindrop::Scene::Entity entity = scene.createEntity();
        }

        void createDebugLayer(){
            // auto& layers = _engine->getLayerManager();
            
            // Raindrop::Layer::Layer debug = layers.createLayer();
            // auto& scene = debug.emplaceModule<Raindrop::Scene::SceneModule>().scene;

            // scene.emplaceBehavior<Raindrop::Behaviors::TagAttacherBehavior>();
            // scene.emplaceBehavior<Raindrop::Behaviors::TransformAttacherBehavior>();
            // scene.emplaceBehavior<Raindrop::Behaviors::HierarchyAttacherBehavior>();
            // scene.emplaceBehavior<Raindrop::Behaviors::HierarchyTransformPropagator>();

            // Raindrop::Scene::Entity entity = scene.createEntity();
        }

        void update(){
            
        }

    private:
        Raindrop::Engine* _engine;
        std::shared_ptr<Raindrop::Window::Window> _window;

        Raindrop::Scheduler::Subscription updateSubscription;
};

int main(){
    Raindrop::Engine engine;
    auto& systems = engine.getSystemManager();

    systems.registerSystem<Raindrop::Window::WindowSystem>();
    systems.registerSystem<Raindrop::Event::EventSystem>();
    // systems.registerSystem<Raindrop::Render::RenderSystem>();
    // systems.registerSystem<Raindrop::Scene::SceneSystem>();
    systems.registerSystem<Testbed>();

    engine.start();

    return 0;
}