#include <Raindrop/Raindrop.hpp>

class Testbed : public Raindrop::System::ISystem{
    public:
        Testbed(){}
        virtual ~Testbed() override = default;

        virtual void initialize(Raindrop::Engine& engine) override{
            _engine = &engine;
            auto& systems = engine.getSystemManager();

            systems.getSystem<Raindrop::Event::EventSystem>()->getManager().subscribe<Raindrop::Window::WindowCloseRequest>(
                [this](const Raindrop::Window::WindowCloseRequest& event) -> bool {
                    if (event.getWindow() == _window){
                        _engine->stop();
                    }
                    return false;
                }
            );

            createWindow();
            createGameplayLayer();
            createDebugLayer();
        }

        virtual std::vector<Dependency> dependencies() const override{
            return {
                Dependency::Make<Raindrop::Event::EventSystem>(),
                Dependency::Make<Raindrop::Scene::SceneSystem>(),
                Dependency::Make<Raindrop::Graphics::RenderSystem>(),
                Dependency::Make<Raindrop::Window::WindowSystem>(),
            };
        }

        virtual void update() override{
            // _engine->stop();
        }

        virtual const char* name() const override{
            return "Testbed";
        }

        void createWindow(){
            auto& systems = _engine->getSystemManager();
            auto windowSys = systems.getSystem<Raindrop::Window::WindowSystem>();

            using Raindrop::Window::WindowFlags;

            Raindrop::Window::WindowConfig config {
                .resolution = {800, 600},
                .position = {0, 0},
                .title = "testbed",
                .flags = {
                    WindowFlags::RESIZABLE
                }
            };
            _window = windowSys->createWindow(config);
        }

        void createGameplayLayer(){
            auto& layers = _engine->getLayerManager();
            
            Raindrop::Layer::Layer gameplay = layers.createLayer();
            auto& scene = gameplay.emplaceModule<Raindrop::Scene::SceneModule>().scene;

            scene.emplaceBehavior<Raindrop::Builtin::Behaviors::TagAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Builtin::Behaviors::TransformAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Builtin::Behaviors::HierarchyAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Builtin::Behaviors::HierarchyTransformPropagator>();

            Raindrop::Scene::Entity entity = scene.createEntity();
        }

        void createDebugLayer(){
            auto& layers = _engine->getLayerManager();
            
            Raindrop::Layer::Layer debug = layers.createLayer();
            auto& scene = debug.emplaceModule<Raindrop::Scene::SceneModule>().scene;

            scene.emplaceBehavior<Raindrop::Builtin::Behaviors::TagAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Builtin::Behaviors::TransformAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Builtin::Behaviors::HierarchyAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Builtin::Behaviors::HierarchyTransformPropagator>();

            Raindrop::Scene::Entity entity = scene.createEntity();
        }

    private:
        Raindrop::Engine* _engine;
        std::shared_ptr<Raindrop::Window::Window> _window;
};

int main(){
    Raindrop::Engine engine;
    auto& systems = engine.getSystemManager();

    systems.emplaceSystem<Raindrop::Window::WindowSystem>();
    systems.emplaceSystem<Raindrop::Event::EventSystem>();
    systems.emplaceSystem<Raindrop::Graphics::RenderSystem>();
    systems.emplaceSystem<Raindrop::Scene::SceneSystem>();
    systems.emplaceSystem<Testbed>();

    engine.run();

    return 0;
}