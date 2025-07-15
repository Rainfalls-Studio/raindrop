#include <Raindrop/Raindrop.hpp>
#include "Raindrop/Builtin/Components/Tag.hpp"

class Testbed : public Raindrop::System::ISystem{
    public:
        Testbed(){}
        virtual ~Testbed() override = default;

        virtual void initialize(Raindrop::Engine& engine) override{
            _engine = &engine;
            auto& systems = engine.getSystemManager();
            systems.getSystem<Raindrop::Event::EventSystem>()->getManager().subscribe<Raindrop::Window::WindowCloseRequest>([&engine](const auto&) -> bool {engine.stop(); return false;});
            createGameplayLayer();
        }

        virtual void postInitialize() override{
        }

        virtual std::vector<Dependency> dependencies() const override{
            return {
                {typeid(Raindrop::Event::EventSystem)},
                {typeid(Raindrop::Scene::SceneSystem)},
            };
        }

        virtual void update() override{
            // _engine->stop();
        }

        virtual const char* name() const override{
            return "Testbed";
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

    private:
        Raindrop::Engine* _engine;
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