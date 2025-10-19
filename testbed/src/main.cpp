#include <Raindrop/Raindrop.hpp>

using namespace Raindrop::Time::literals;

class Testbed : public Raindrop::Modules::IModule{
    public:

        Testbed(){}
        virtual ~Testbed() override = default;

        virtual Raindrop::Modules::Result initialize(Raindrop::Modules::InitHelper& init) override{
            _engine = &init.engine();

            auto filesystem = init.getDependencyAs<Raindrop::Filesystem::FilesystemModule>("Filesystem");
            Raindrop::Filesystem::Path parent = filesystem->getExecutableDirectory().parent();

            filesystem->mount<Raindrop::Filesystem::FolderProvider>("{root}", 10, parent);

            createWindow();
            createGameplayLayer();
            createDebugLayer();
            setupGameplayLoop();

            return Raindrop::Modules::Result::Success();
        }

        virtual Raindrop::Modules::DependencyList dependencies() const noexcept override{
            using Raindrop::Modules::Dependency;

            return {
                Dependency("Event"),
                Dependency("RenderOutput"),
                Dependency("RenderGraph"),
                Dependency("Window"),
                Dependency("Scene"),
                Dependency("ImGui"),
                Dependency("Filesystem")
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
            auto& modules = _engine->getModuleManager();

            auto [windowMod, eventMod, renderOutputMod] = modules.getModulesAs<
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

            // register the window as output "main"
            renderOutputMod->registerOutput<Raindrop::Render::WindowRenderOutput>("main", _window);
            
            // subscribe to event 'WindowCloseRequest'. In which case, stop the engine
            eventMod->getManager().subscribe<Raindrop::Window::Events::WindowCloseRequest>(
                [this](const Raindrop::Window::Events::WindowCloseRequest& event) -> bool {
                    if (event.getWindow() == _window){
                        _engine->stop();
                    }
                    return false;
                }
            );
        }

        void setupGameplayLoop(){
            auto& scheduler = _engine->getScheduler();

            Raindrop::Scheduler::Loop updateLoop = scheduler.createLoop("Gameplay update")
                .setPeriod(30_Hz)
                .addStage<Raindrop::Window::EventStage>();
                // .addStage<Raindrop::Scene::SceneUpdateStage>(_gameplay) // runs scene behaviors
                // .addStage<Raindrop::Script::ScriptUpdateStage>(_gameplay); // run script updates

            Raindrop::Scheduler::Loop physicsLoop = scheduler.createLoop("Gameplay physics")
                .setPeriod(30_Hz);
                // .addStage<Raindrop::Physics::PhysicsUpdateStage>(_gameplay);
            
            Raindrop::Scheduler::Loop renderLoop = scheduler.createLoop("Render")
                .setPeriod(4_Hz)
                // .addStage<Raindrop::Window::EventStage>()
                .addStage<Raindrop::Render::RenderGraphRecordStage>(_gameplay)
                // .addStage<Raindrop::Render::RenderGraphRecordStage>(_hud)
                .addStage<Raindrop::Render::RenderGraphRenderStage>()
                .addStage<Raindrop::Render::ImGuiStage>("main")
                .addStage<Raindrop::Render::PresentRenderOutputStage>("main");

            scheduler.run(updateLoop);
            scheduler.run(renderLoop);
            // scheduler.run(updateLoop);
            // scheduler.run(physicsLoop);
        }

        void createGameplayLayer(){
            auto& layers = _engine->getLayerManager();
            auto sceneModule = _engine->getModuleManager().getModuleAs<Raindrop::Scene::SceneModule>("Scene");
            
            _gameplay = layers.createLayer();
            auto& scene = sceneModule->emplaceTrait(_gameplay).scene;

            scene.emplaceBehavior<Raindrop::Behaviors::TagAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::TransformAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::HierarchyAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::HierarchyTransformPropagator>();
        }

        void createDebugLayer(){
            auto& layers = _engine->getLayerManager();
            auto sceneModule = _engine->getModuleManager().getModuleAs<Raindrop::Scene::SceneModule>("Scene");
            
            _debug = layers.createLayer();
            auto& scene = sceneModule->emplaceTrait(_debug).scene;

            scene.emplaceBehavior<Raindrop::Behaviors::TagAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::TransformAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::HierarchyAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::HierarchyTransformPropagator>();
        }

        void update(){
            static size_t counter = 0;
            if (counter++ == 200){
                // TEST
                // _engine->getModuleManager().registerModule<Raindrop::Render::RenderOutputModule>();
            }
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

        Raindrop::Layers::Layer _gameplay;
        Raindrop::Layers::Layer _debug;
};

#include <signal.h>

Raindrop::Engine* enginePtr;

void interupt_handler(int){
    spdlog::warn("Keyboard interupt !");
    enginePtr->stop();
}

void setup_interuption_handler(){
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = interupt_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
}


int main(){
    spdlog::set_level(spdlog::level::trace);

    Raindrop::Engine engine;
    enginePtr = &engine;
    auto& modules = engine.getModuleManager();

    modules.registerModule<Raindrop::Window::WindowModule>();
    modules.registerModule<Raindrop::Event::EventModule>();
    modules.registerModule<Raindrop::Render::RenderCoreModule>();
    modules.registerModule<Raindrop::Render::RenderOutputModule>();
    modules.registerModule<Raindrop::Render::RenderGraphModule>();
    modules.registerModule<Raindrop::Scene::SceneModule>();
    modules.registerModule<Raindrop::Filesystem::FilesystemModule>();
    modules.registerModule<Raindrop::Asset::AssetModule>();
    modules.registerModule<Raindrop::Render::ImGuiModule>();
    modules.registerModule<Testbed>();
    
    setup_interuption_handler();

    engine.start();

    return 0;
}