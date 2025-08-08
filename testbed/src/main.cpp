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

            {
                auto renderGraph = init.getDependencyAs<Raindrop::Render::RenderGraphModule>("RenderGraph");
                
                renderGraph->getFrameGraph()->createPass("WindowClear", crg::RunnablePassCreator(
                    [this](const crg::FramePass& pass, crg::GraphContext& ctx, crg::RunnableGraph& graph) -> crg::RunnablePassPtr {
                        return std::make_unique<Raindrop::Render::BlitToRenderOutputPass>(
                            *_engine,
                            pass,
                            ctx,
                            graph,
                            "main"
                        );
                    }
                ));
            }

            // createWindow();
            createGameplayLayer();
            createDebugLayer();
            setupGameplayLoop();

            // auto& scheduler = _engine->getScheduler();

            // updateSubscription = scheduler.subscribe([this]{update();}, Raindrop::Scheduler::Priority::UPDATE);

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

        void setupGameplayLoop(){
            auto& scheduler = _engine->getScheduler();

            Raindrop::Scheduler::Loop updateLoop = scheduler.createLoop("Gameplay update")
                .setPeriod(30_Hz);
                // .addStage<Raindrop::Scene::SceneUpdateStage>(_gameplay) // runs scene behaviors
                // .addStage<Raindrop::Script::ScriptUpdateStage>(_gameplay); // run script updates

            Raindrop::Scheduler::Loop physicsLoop = scheduler.createLoop("Gameplay physics")
                .setPeriod(30_Hz);
                // .addStage<Raindrop::Physics::PhysicsUpdateStage>(_gameplay);
            
            Raindrop::Scheduler::Loop renderLoop = scheduler.createLoop("Render")
                .setPeriod(4_Hz);
                // .addStage<Raindrop::Render::RenderStage>(_gameplay)
                // .addStage<Raindrop::Render::RenderStage>(_HUD)
                // .addStage<Raindrop::Render::PresentStage>("main"); // present to render output named "gameplay" (could be a window or a buffer set up by an editor layer)

            scheduler.run(renderLoop);
        }

        void createGameplayLayer(){
            auto& layers = _engine->getLayerManager();
            auto sceneModule = _engine->getModuleManager().getModuleAs<Raindrop::Scene::SceneModule>("Scene");
            
            Raindrop::Layers::Layer gameplay = layers.createLayer();
            auto& scene = sceneModule->emplaceTrait(gameplay).scene;

            scene.emplaceBehavior<Raindrop::Behaviors::TagAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::TransformAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::HierarchyAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::HierarchyTransformPropagator>();
        }

        void createDebugLayer(){
            auto& layers = _engine->getLayerManager();
            auto sceneModule = _engine->getModuleManager().getModuleAs<Raindrop::Scene::SceneModule>("Scene");
            
            Raindrop::Layers::Layer debug = layers.createLayer();
            auto& scene = sceneModule->emplaceTrait(debug).scene;

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
        
        // Raindrop::Scheduler::Subscription updateSubscription;
};

int main(){
    spdlog::set_level(spdlog::level::trace);

    Raindrop::Engine engine;
    auto& modules = engine.getModuleManager();

    modules.registerModule<Raindrop::Window::WindowModule>();
    modules.registerModule<Raindrop::Event::EventModule>();
    modules.registerModule<Raindrop::Render::RenderCoreModule>();
    modules.registerModule<Raindrop::Render::RenderOutputModule>();
    modules.registerModule<Raindrop::Render::RenderGraphModule>();
    modules.registerModule<Raindrop::Render::RenderSchedulerModule>();
    modules.registerModule<Raindrop::Scene::SceneModule>();
    modules.registerModule<Raindrop::Filesystem::FilesystemModule>();
    modules.registerModule<Raindrop::Asset::AssetModule>();
    modules.registerModule<Testbed>();

    engine.start();

    return 0;
}