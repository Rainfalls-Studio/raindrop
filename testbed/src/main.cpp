#include <Raindrop/Raindrop.hpp>


class Testbed : public Raindrop::Modules::IModule{
    public:

        Testbed(){}
        virtual ~Testbed() override = default;

        virtual Raindrop::Modules::Result initialize(Raindrop::Modules::InitHelper& init) override{
            _engine = &init.engine();

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
                Dependency("RenderOutput"),
                Dependency("RenderGraph"),
                Dependency("Window")
            };
        }

        virtual std::string name() const noexcept override{
            return "Testbed";
        }

        virtual bool critical() const noexcept override {
            return true;
        }

        void createWindow(){
            // Get Modules
            auto& Modules = _engine->getModuleManager();

            auto [windowMod, eventMod, renderOutputMod] = Modules.getModulesAs<
                Raindrop::Window::WindowModule,
                Raindrop::Event::EventModule,
                Raindrop::Render::RenderOutputModule>(
                    "Window",
                    "Event",
                    "RenderOutput"
                );

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
            _window = windowMod->createWindow(config);

            renderOutputMod->registerOutput<Raindrop::Render::WindowRenderOutput>("main", _window);

            eventMod->getManager().subscribe<Raindrop::Window::Events::WindowCloseRequest>(
                [this](const Raindrop::Window::Events::WindowCloseRequest& event) -> bool {
                    if (event.getWindow() == _window){
                        _engine->stop();
                    }
                    return false;
                }
            );
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
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }


        virtual Raindrop::Modules::Result dependencyReload(const Name& dependency) override {
            spdlog::info(dependency);
            
            if (dependency == "Window"){
                createWindow();
                return Raindrop::Modules::Result::Success();
            }
            return Raindrop::Modules::Result::Success();
        }

    private:
        Raindrop::Engine* _engine;
        std::shared_ptr<Raindrop::Window::Window> _window;

        Raindrop::Scheduler::Subscription updateSubscription;
};

int main(){
    spdlog::set_level(spdlog::level::trace);

    Raindrop::Engine engine;
    auto& Modules = engine.getModuleManager();

    Modules.registerModule<Raindrop::Window::WindowModule>();
    Modules.registerModule<Raindrop::Event::EventModule>();
    Modules.registerModule<Raindrop::Render::RenderCoreModule>();
    Modules.registerModule<Raindrop::Render::RenderOutputModule>();
    Modules.registerModule<Raindrop::Render::RenderGraphModule>();
    // Modules.registerModule<Raindrop::Scene::SceneSystem>();
    Modules.registerModule<Testbed>();

    engine.start();

    return 0;
}