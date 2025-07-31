#include <Raindrop/Raindrop.hpp>


class Testbed : public Raindrop::Modules::IModule{
    public:

        Testbed(){}
        virtual ~Testbed() override = default;

        virtual Raindrop::Modules::Result initialize(Raindrop::Modules::InitHelper& init) override{
            _engine = &init.engine();

            auto& modules = init.getModules();

            modules.getModuleAs<Raindrop::Event::EventModule>("Event")->getManager().subscribe<Raindrop::Window::Events::WindowCloseRequest>(
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

            auto& scheduler = _engine->getScheduler();

            updateSubscription = scheduler.subscribe([this]{update();}, Raindrop::Scheduler::Priority::UPDATE);

            return Raindrop::Modules::Result::Success();
        }

        virtual Raindrop::Modules::DependencyList dependencies() const noexcept override{
            using Raindrop::Modules::Dependency;

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
            // Get Modules
            auto& Modules = _engine->getModuleManager();

            // auto [windowSys, renderSys] = Modules.getModulesAs<
            //     Raindrop::Window::WindowSystem,
            //     Raindrop::Render::RenderSystem>(
            //         "Window",
            //         "Render"
            //     );

            auto windowSys = Modules.getModuleAs<Raindrop::Window::WindowModule>("Window");

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
    auto& Modules = engine.getModuleManager();

    Modules.registerModule<Raindrop::Window::WindowModule>();
    Modules.registerModule<Raindrop::Event::EventModule>();
    // Modules.registerSystem<Raindrop::Render::RenderSystem>();
    // Modules.registerSystem<Raindrop::Scene::SceneSystem>();
    Modules.registerModule<Testbed>();

    engine.start();

    return 0;
}