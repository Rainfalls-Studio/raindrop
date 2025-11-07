#include <Raindrop/Raindrop.hpp>
#include "Editor.hpp"
#include "Content.hpp"
// #include "Planet.hpp"
#include "planet/PlanetServiceBehavior.hpp"

using namespace Raindrop::Time::literals;

class Sim : public Raindrop::Modules::IModule{
    public:
        Sim(){}
        virtual ~Sim() override{
            _bufferCtx.reset();
        }

        virtual Raindrop::Modules::Result initialize(Raindrop::Modules::InitHelper& init) override{
            _engine = &init.engine();

            _filesystem= init.getDependencyAs<Raindrop::Filesystem::FilesystemModule>("Filesystem");
            _assets = init.getDependencyAs<Raindrop::Asset::AssetModule>("Asset");
            _renderCore = init.getDependencyAs<Raindrop::Render::RenderCoreModule>("RenderCore");
            _imGuiModule = init.getDependencyAs<Raindrop::ImGui::ImGuiModule>("ImGui");
            _outputs = init.getDependencyAs<Raindrop::Render::RenderOutputModule>("RenderOutput");

            setupMounts();
            registerFactories();
            createWindow();
            createGameplayLayer();
            createBufferContext();
            createOffscreenBuffer();
            createImGuiContext();

            setupLoops();

            return Raindrop::Modules::Result::Success();
        }

        void setupMounts(){
            Raindrop::Filesystem::Path parent = _filesystem->getExecutableDirectory().parent();

            _filesystem->mount<Raindrop::Filesystem::FolderProvider>("{root}", 10, parent);
            _filesystem->mount<Raindrop::Filesystem::FolderProvider>("{resources}", 10, parent/"resources");

            spdlog::info("dir : {}", _filesystem->getExecutableDirectory());
            spdlog::info("aa : {}", _filesystem->exists("{resources}/shaders"));
        }

        void registerFactories(){
            _shaderFactory = _assets->emplaceFactory<Raindrop::Render::Shader, Raindrop::Render::ShaderFactory>(*_engine);
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
                Dependency("Filesystem"),
                Dependency("Asset"),
            };
        }

        virtual std::string name() const noexcept override{
            return "Sim";
        }

        virtual bool critical() const noexcept override {
            return true;
        }

        void createOffscreenBuffer(){
            _offscreenBuffer =  _outputs->createOutput<Raindrop::Render::BufferRenderOutput>(
                "offscreen",
                _renderCore,
                Raindrop::Render::BufferRenderOutput::Info{
                    vk::Extent2D{
                        1080,
                        720
                    },
                    {
                        Raindrop::Render::BufferRenderOutput::AttachmentDescription{
                            "color attachment",
                            vk::Format::eR8G8B8A8Unorm,
                            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled,
                            vk::ClearValue{
                                vk::ClearColorValue{
                                    0.f, 0.f, 0.f, 1.f
                                }
                            },
                            vk::AttachmentLoadOp::eClear,
                            vk::AttachmentStoreOp::eStore
                        }
                    },
                    Raindrop::Render::BufferRenderOutput::AttachmentDescription{
                        "depth attachment",
                        vk::Format::eD16Unorm,
                        vk::ImageUsageFlagBits::eDepthStencilAttachment | vk::ImageUsageFlagBits::eSampled,
                        vk::ClearValue{
                            vk::ClearDepthStencilValue{
                                1.f, 0
                            }
                        },
                        vk::AttachmentLoadOp::eClear,
                        vk::AttachmentStoreOp::eStore
                    }
                }
            );
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
                .setPeriod(100_Hz)
                .addStage<Raindrop::Window::EventStage>();
                // .addStage<Raindrop::Scene::SceneUpdateStage>(_gameplay) // runs scene behaviors
                // .addStage<Raindrop::Script::ScriptUpdateStage>(_gameplay); // run script updates
            
            Raindrop::Scheduler::Loop renderLoop = scheduler.createLoop("Render")
                .addStage<Raindrop::Render::IRenderOutput::AcquireStage>(_windowOutput, _bufferCtx)
                    .addStage<Raindrop::Render::RenderCommandContext::BeginStage>(_bufferCtx)

                        // .addStage<Scene::PhaseStage>(_scene, Stage::PRE_RENDER)

                        // offscreen render 
                        .addStage<Raindrop::Render::IRenderOutput::AcquireStage>(_offscreenBuffer, _bufferCtx)
                            .addStage<Raindrop::Render::IRenderOutput::BeginStage>(_offscreenBuffer, _bufferCtx)
                                // .addStage<Planet::RenderStage>(_scene, _offscreenBuffer, _bufferCtx)
                            .addStage<Raindrop::Render::IRenderOutput::EndStage>(_offscreenBuffer, _bufferCtx)
                        .addStage<Raindrop::Render::IRenderOutput::PresentStage>(_offscreenBuffer, _bufferCtx)
                        
                        // Swapchain render
                        .addStage<Raindrop::Render::IRenderOutput::BeginStage>(_windowOutput, _bufferCtx)

                            .addStage<Raindrop::ImGui::BeginStage>(_imGuiCtx)
                                .addStage<Editor>()
                                .addStage<ContentStage>(_offscreenBuffer)
                            .addStage<Raindrop::ImGui::EndStage>(_imGuiCtx)

                            .addStage<Raindrop::ImGui::RenderStage>(_imGuiCtx, _bufferCtx)

                        .addStage<Raindrop::Render::IRenderOutput::EndStage>(_windowOutput, _bufferCtx)
                    .addStage<Raindrop::Render::RenderCommandContext::EndStage>(_bufferCtx)
                    .addStage<Raindrop::Render::RenderCommandContext::SubmitStage>(_bufferCtx, _renderCore->graphicsQueue())
                .addStage<Raindrop::Render::IRenderOutput::PresentStage>(_windowOutput, _bufferCtx);

            scheduler.run(updateLoop);
            scheduler.run(renderLoop);
        }

        void createGameplayLayer(){
            auto& layers = _engine->getLayerManager();
            auto sceneModule = _engine->getModuleManager().getModuleAs<Raindrop::Scene::SceneModule>("Scene");
            
            _gameplay = layers.createLayer();
            auto& scene = sceneModule->emplaceTrait(_gameplay).scene;

            // Not ticked behaviors
            scene.emplaceBehavior<Raindrop::Behaviors::TagAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::TransformAttacherBehavior>();
            scene.emplaceBehavior<Raindrop::Behaviors::HierarchyAttacherBehavior>();
            
            _preUpdateStage = scene.createStage("Pre Update");
            _updateStage = scene.createStage("Update");
            _renderStage = scene.createStage("Render");

            scene.addToStage(_preUpdateStage, scene.emplaceBehavior<Raindrop::Behaviors::HierarchyTransformPropagator>());

            scene.addToStage(_updateStage, scene.emplaceBehavior<Planet::ServiceBehavior>());
            // scene.emplaceBehavior<Planet::PreRenderBehavior>().in(Stage::PreRender); // collects visible chunks and 
            // scene.emplaceBehavior<Planet::RenderBehavior>().in(Stage::Render);

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

        Raindrop::Scene::StageID _preUpdateStage;
        Raindrop::Scene::StageID _updateStage;
        Raindrop::Scene::StageID _renderStage;

        std::shared_ptr<Raindrop::Filesystem::FilesystemModule> _filesystem;
        std::shared_ptr<Raindrop::Render::RenderCoreModule> _renderCore;
        std::shared_ptr<Raindrop::Render::RenderOutputModule> _outputs;
        std::shared_ptr<Raindrop::ImGui::ImGuiModule> _imGuiModule;
        std::shared_ptr<Raindrop::ImGui::ImGuiContext> _imGuiCtx;
        std::shared_ptr<Raindrop::Asset::AssetModule> _assets;


        std::shared_ptr<Raindrop::Render::RenderCommandContext> _bufferCtx;

        std::shared_ptr<Raindrop::Window::Window> _window;
        std::shared_ptr<Raindrop::Render::IRenderOutput> _windowOutput;

        std::shared_ptr<Raindrop::Render::BufferRenderOutput> _offscreenBuffer;

        std::shared_ptr<Raindrop::Scene::Scene> _scene;

        std::shared_ptr<Raindrop::Render::ShaderFactory> _shaderFactory;

        Raindrop::Layers::Layer _gameplay;
        Raindrop::Layers::Layer _debug;
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