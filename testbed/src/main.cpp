#include <Raindrop/Raindrop.hpp>

class Testbed : public Raindrop::System::ISystem{
    public:
        Testbed(){}
        virtual ~Testbed() override = default;

        virtual void initialize(Raindrop::Engine& engine) override{
            _engine = &engine;
            auto& systems = engine.getSystemManager();
            systems.getSystem<Raindrop::Event::EventSystem>()->getManager().subscribe<Raindrop::Window::WindowCloseRequest>([&engine](const auto&) -> bool {engine.stop(); return false;});
        }

        virtual std::vector<Dependency> dependencies() const override{
            return {
                {typeid(Raindrop::Event::EventSystem)}
            };
        }

        virtual void update() override{
            // _engine->stop();
        }

        virtual const char* name() const override{
            return "Testbed";
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

    auto& layers = engine.getLayerManager();
    
    Raindrop::Layer::Layer gameplay = layers.createLayer();
    auto& scene = gameplay.emplaceModule<Raindrop::Scene::LayerModule>().scene;

    Raindrop::Scene::Entity entity = scene.createEntity();
    
    engine.run();

    return 0;
}