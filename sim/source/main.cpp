#include <Raindrop/Raindrop.hpp>
#include "Editor.hpp"
#include "Content.hpp"

using namespace Raindrop::Time::literals;

class Sim : public Raindrop::Modules::IModule{
    public:
        Sim(){}
        virtual ~Sim() override = default;

        virtual Raindrop::Modules::Result initialize(Raindrop::Modules::InitHelper& init) override{
            _engine = &init.engine();

            auto filesystem = init.getDependencyAs<Raindrop::Filesystem::FilesystemModule>("Filesystem");
            Raindrop::Filesystem::Path parent = filesystem->getExecutableDirectory().parent();

            filesystem->mount<Raindrop::Filesystem::FolderProvider>("{root}", 10, parent);

            _renderCore = init.getDependencyAs<Raindrop::Render::RenderCoreModule>("RenderCore");
            _imGuiModule = init.getDependencyAs<Raindrop::ImGui::ImGuiModule>("ImGui");

            createWindow();
            createRenderGraph();
            createGameplayLayer();
            createBufferContext();
            createImGuiContext();
            setupLoops();

            return Raindrop::Modules::Result::Success();
        }

        virtual Raindrop::Modules::DependencyList dependencies() const noexcept override{
            using Raindrop::Modules::Dependency;

            return {
                Dependency("Event"),
                Dependency("RenderCore"),
                Dependency("RenderOutput"),
                Dependency("RenderGraph"),
                Dependency("Window"),
                Dependency("Scene"),
                Dependency("ImGui"),
                Dependency("Filesystem")
            };
        }

        virtual std::string name() const noexcept override{
            return "Sim";
        }

        virtual bool critical() const noexcept override {
            return true;
        }

        void createBufferContext(){
            _bufferCtx = std::make_shared<Raindrop::Render::RenderCommandContext>(
                _renderCore,
                Raindrop::Render::Queue::Type::GRAPHICS,
                2,
                "main graphics buffer context"
            );
        }

        void createImGuiContext(){
            _imGuiCtx = _imGuiModule->createContext(_windowOutput);
        }
        

        void createRenderGraph(){
            // auto& modules = _engine->getModuleManager();
            // auto renderGraphMod = modules.getModuleAs<Raindrop::Render::RenderGraphModule>("RenderGraph");

            // // the name does not serve as id but as debug and user identification
            // _graph = renderGraphMod->createGraph("graph");
            // auto& frameGraph = _graph->get();

            // // auto& content = frameGraph.createPass("Content",
            // //     [&]( const crg::FramePass& pass, crg::GraphContext& ctx, crg::RunnableGraph& runGraph ) -> crg::RunnablePassPtr {
            // //         return std::make_unique<ContentPass>(pass, ctx, runGraph, contentImage);
            // //     }
            // // );

            // auto& swapchainPass = frameGraph.createPass("Main render target render", 
            //     [&]( const crg::FramePass& pass, crg::GraphContext& ctx, crg::RunnableGraph& runGraph ) -> crg::RunnablePassPtr {
            //         using Raindrop::Render::MakeStep;
            //         using Raindrop::Render::MakeSteps;
            //         auto imguiMod = _engine->getModuleManager().getModuleAs<Raindrop::Render::ImGuiModule>("ImGui");
                    
            //         return std::make_unique<Raindrop::Render::RenderSequencePass>(pass, ctx, runGraph,
            //             MakeSteps(
            //                 MakeStep<Raindrop::Render::RenderOutputBeginRenderPass>(_windowOutput, _graph),
            //                 MakeStep<Raindrop::Render::ImGuiRenderStep>(imguiMod, _windowOutput, "main"),
            //                 MakeStep<Raindrop::Render::RenderOutputEndRenderPass>(_windowOutput)
            //             )
            //         );
            //     }
            // );

            // // swapchainPass.dependsOn(content);
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
                .resolution = {2560, 1400},
                .position = {0, 0},
                .title = "Sim",
                .flags = {
                    WindowFlags::RESIZABLE
                }
            };
            _window = windowMod->createWindow(config);

            // register the window as output "main"
            _windowOutput = renderOutputMod->createOutput<Raindrop::Render::WindowRenderOutput>("main", _window);
            
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

        void setupLoops(){
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
                .addStage<Raindrop::Render::IRenderOutput::AcquireStage>(_windowOutput, _bufferCtx)
                .addStage<Raindrop::ImGui::BeginStage>(_imGuiCtx)
                .addStage<Raindrop::ImGui::EndStage>(_imGuiCtx)
                .addStage<Raindrop::Render::RenderCommandContext::BeginStage>(_bufferCtx)
                .addStage<Raindrop::Render::IRenderOutput::BeginStage>(_windowOutput, _bufferCtx)
                .addStage<Raindrop::ImGui::RenderStage>(_imGuiCtx, _bufferCtx)
                .addStage<Raindrop::Render::IRenderOutput::EndStage>(_windowOutput, _bufferCtx)
                .addStage<Raindrop::Render::RenderCommandContext::EndStage>(_bufferCtx)
                .addStage<Raindrop::Render::RenderCommandContext::SubmitStage>(_bufferCtx, _renderCore->graphicsQueue())
                .addStage<Raindrop::Render::IRenderOutput::PresentStage>(_windowOutput, _bufferCtx);

            scheduler.run(updateLoop);
            scheduler.run(renderLoop);
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

        std::shared_ptr<Raindrop::Render::RenderCoreModule> _renderCore;
        std::shared_ptr<Raindrop::ImGui::ImGuiModule> _imGuiModule;


        std::shared_ptr<Raindrop::Render::RenderCommandContext> _bufferCtx;
        std::shared_ptr<Raindrop::ImGui::ImGuiContext> _imGuiCtx;

        std::shared_ptr<Raindrop::Window::Window> _window;
        std::shared_ptr<Raindrop::Render::IRenderOutput> _windowOutput;

        Raindrop::Layers::Layer _gameplay;
        Raindrop::Layers::Layer _debug;
        std::shared_ptr<Raindrop::Render::RenderGraph> _graph;
};


int main(){
    Raindrop::Engine engine;

    auto& modules = engine.getModuleManager();

    modules.registerModule<Raindrop::Window::WindowModule>();
    modules.registerModule<Raindrop::Event::EventModule>();
    modules.registerModule<Raindrop::Render::RenderCoreModule>();
    modules.registerModule<Raindrop::Render::RenderOutputModule>();
    modules.registerModule<Raindrop::Render::RenderGraphModule>();
    modules.registerModule<Raindrop::Scene::SceneModule>();
    modules.registerModule<Raindrop::Filesystem::FilesystemModule>();
    modules.registerModule<Raindrop::Asset::AssetModule>();
    modules.registerModule<Raindrop::ImGui::ImGuiModule>();
    modules.registerModule<Sim>();

    engine.start();
    
    return 0;
}