#include <Raindrop/Raindrop.hpp>
#include "Content.hpp"
// #include "Planet.hpp"
#include "planet/PlanetServiceBehavior.hpp"
#include "planet/PlanetRenderBehavior.hpp"
#include "planet/PlanetComponent.hpp"
#include "planet/PlanetUpdateBehavior.hpp"
#include "CameraUpdateBehavior.hpp"
#include "Editor/Editor.hpp"
#include "Editor/RenderStage.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <imgui_stdlib.h>

using namespace Raindrop::Time::literals;

class Sim : public Raindrop::Modules::IModule{
    public:
        Sim(){}
        virtual ~Sim() override{
            _scene->shutdown();
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
            createBufferContext();
            createOffscreenBuffer();
            createImGuiContext();

            createGameplayLayer();

            setupEditor();
            setupLoops();


            return Raindrop::Modules::Result::Success();
        }

        void setupEditor(){

            _editor = std::make_shared<Editor::Editor>();
            _editor->registerScene(_scene);

            _editor->registerComponent(Editor::ComponentInfo{
                .name = "Tag",
                .add = [](Editor::Entity& e) -> std::expected<void, Raindrop::Error> {
                    try{
                        e.emplace<Raindrop::Components::Tag>();
                    } catch (const std::exception& e){
                        return {}; // TODO: setup error codes
                    }
                    return {};
                },
                .has = [](Editor::Entity& e) -> bool {
                    return e.hasAny<Raindrop::Components::Tag>();
                },
                .drawUI = [](Editor::Entity& e) -> void {
                    auto& tag = e.get<Raindrop::Components::Tag>();

                    ImGui::InputText("Tag", &tag.tag);
                }
            });

            _editor->registerComponent(Editor::ComponentInfo{
                .name = "Transform",
                .add = [](Editor::Entity& e) -> std::expected<void, Raindrop::Error> {
                    try{
                        e.emplace<Raindrop::Components::Transform>();
                    } catch (const std::exception& e){
                        return {}; // TODO: setup error codes
                    }
                    return {};
                },
                .has = [](Editor::Entity& e) -> bool {
                    return e.hasAny<Raindrop::Components::Transform>();
                },
                .drawUI = [](Editor::Entity& e) -> void {
                    auto& transform = e.get<Raindrop::Components::Transform>();

                    bool updated = false;
                    updated |= ImGui::DragFloat3("Translation", glm::value_ptr(transform.translation), 0.1f);
                    updated |= ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.1f);

                    glm::vec3 eulerAngles = glm::eulerAngles(transform.rotation);

                    if (ImGui::DragFloat3("Axis", glm::value_ptr(eulerAngles), 0.1f)) {
                        updated = true;

                        transform.rotation = glm::normalize(glm::quat(eulerAngles));
                    }

                    if (updated){
                        transform.dirty = true;
                    }
                }
            });

            _editor->registerComponent(Editor::ComponentInfo{
                .name = "Hierarchy",
                .add = [](Editor::Entity& e) -> std::expected<void, Raindrop::Error> {
                    try{
                        e.emplace<Raindrop::Components::Hierarchy>();
                    } catch (const std::exception& e){
                        return {}; // TODO: setup error codes
                    }
                    return {};
                },
                .has = [](Editor::Entity& e) -> bool {
                    return e.hasAny<Raindrop::Components::Hierarchy>();
                },
                .drawUI = [](Editor::Entity& e) -> void {
                    auto& hierarchy = e.get<Raindrop::Components::Hierarchy>();

                    if (hierarchy.parent == Raindrop::Scene::INVALID_ENTITY_HANDLE){
                        ImGui::Text("parent : No parent");
                    } else {
                        ImGui::Text("parent : %d", hierarchy.parent);
                    }

                    if (ImGui::BeginTable("", 1)){
                        for (auto child : hierarchy.children){
                            ImGui::TableNextRow();
                            ImGui::TableNextColumn();
                            ImGui::Text("%d", child);
                        }
                        ImGui::EndTable();
                    }
                }
            });

            
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
                .addStage<Raindrop::Window::EventStage>()
                .addStage<Raindrop::Scene::PhaseExecutionStage>(_scene, phase.preUpdate)
                .addStage<Raindrop::Scene::PhaseExecutionStage>(_scene, phase.update)
                .addStage<Raindrop::Scene::PhaseExecutionStage>(_scene, phase.postUpdate);
            
            Raindrop::Scheduler::Loop renderLoop = scheduler.createLoop("Render")
                .addStage<Raindrop::Scene::PhaseExecutionStage>(_scene, phase.preRender)
                .addStage<Raindrop::Render::IRenderOutput::AcquireStage>(_windowOutput, _bufferCtx)
                    .addStage<Raindrop::Render::RenderCommandContext::BeginStage>(_bufferCtx)

                        // .addStage<Raindrop::Scene::PhaseExecutionStage>(_scene, _renderPhase)


                        // offscreen render 
                        .addStage<Raindrop::Render::IRenderOutput::AcquireStage>(_offscreenBuffer, _bufferCtx)
                            .addStage<Raindrop::Render::IRenderOutput::BeginStage>(_offscreenBuffer, _bufferCtx)

                                .addStage<Raindrop::Scene::PhaseExecutionStage>(_scene, phase.render)

                            .addStage<Raindrop::Render::IRenderOutput::EndStage>(_offscreenBuffer, _bufferCtx)
                        .addStage<Raindrop::Render::IRenderOutput::PresentStage>(_offscreenBuffer, _bufferCtx)
                        
                        // Swapchain render

                        .addStage<Raindrop::Render::IRenderOutput::BeginStage>(_windowOutput, _bufferCtx)

                            .addStage<Raindrop::ImGui::BeginStage>(_imGuiCtx)
                                .addStage<Editor::RenderStage>(_editor)
                                .addStage<ContentStage>(_offscreenBuffer, _scene)
                                // .addStage<Raindrop::Scene::PhaseExecutionStage>(_scene, _imGuiPhase)
                            .addStage<Raindrop::ImGui::EndStage>(_imGuiCtx)

                            .addStage<Raindrop::ImGui::RenderStage>(_imGuiCtx, _bufferCtx)

                        .addStage<Raindrop::Render::IRenderOutput::EndStage>(_windowOutput, _bufferCtx)
                    .addStage<Raindrop::Render::RenderCommandContext::EndStage>(_bufferCtx)
                    .addStage<Raindrop::Render::RenderCommandContext::SubmitStage>(_bufferCtx, _renderCore->graphicsQueue())
                .addStage<Raindrop::Render::IRenderOutput::PresentStage>(_windowOutput, _bufferCtx)
                .addStage<Raindrop::Scene::PhaseExecutionStage>(_scene, phase.postRender);


            scheduler.run(updateLoop);
            scheduler.run(renderLoop);
        }

        void createGameplayLayer(){
            auto& layers = _engine->getLayerManager();
            auto sceneModule = _engine->getModuleManager().getModuleAs<Raindrop::Scene::SceneModule>("Scene");
            
            _gameplay = layers.createLayer();
            // auto& scene = sceneModule->emplaceTrait(_gameplay).scene;

            _scene = std::make_shared<Raindrop::Scene::Scene>();
            _scene->initialize(*_engine);

            // Not ticked behaviors
            _scene->emplaceBehavior<Raindrop::Behaviors::TagAttacherBehavior>();
            _scene->emplaceBehavior<Raindrop::Behaviors::TransformAttacherBehavior>();
            _scene->emplaceBehavior<Raindrop::Behaviors::HierarchyAttacherBehavior>();
            _scene->emplaceBehavior<Raindrop::Behaviors::FrameSnapshotService>();
            _scene->emplaceBehavior<Planet::ServiceBehavior>();
            
            phase.preUpdate = _scene->createPhase("Pre Update");
            phase.update = _scene->createPhase("Update");
            phase.postUpdate = _scene->createPhase("Post Update");

            phase.preRender = _scene->createPhase("Pre Render");
            phase.render = _scene->createPhase("Render");
            phase.postRender = _scene->createPhase("Post Render");

            _scene->addToPhase(phase.preUpdate, _scene->emplaceBehavior<Raindrop::Behaviors::FrameSnapshotService::LockWrite>());
            _scene->addToPhase(phase.preUpdate, _scene->emplaceBehavior<Raindrop::Behaviors::HierarchyTransformPropagator>());
            _scene->addToPhase(phase.update, _scene->emplaceBehavior<Planet::UpdateBehavior>());
            _scene->addToPhase(phase.update, _scene->emplaceBehavior<CameraUpdateBehavior>());
            _scene->addToPhase(phase.postUpdate, _scene->emplaceBehavior<Raindrop::Behaviors::FrameSnapshotService::ReleaseWrite>());
            
            _scene->addToPhase(phase.preRender, _scene->emplaceBehavior<Raindrop::Behaviors::FrameSnapshotService::LockRead>());
            _scene->addToPhase(phase.render, _scene->emplaceBehavior<Planet::RenderBehavior>(_offscreenBuffer, _bufferCtx));
            _scene->addToPhase(phase.postRender, _scene->emplaceBehavior<Raindrop::Behaviors::FrameSnapshotService::ReleaseRead>());

            // scene.emplaceBehavior<Planet::PreRenderBehavior>().in(Stage::PreRender); // collects visible chunks and 
            // scene.emplaceBehavior<Planet::RenderBehavior>().in(Stage::Render);

            for (int i=0; i<5; i++){
                auto planet = _scene->createEntity();
                planet.emplace<Planet::PlanetComponent>();
                planet.get<Raindrop::Components::Tag>().tag = "Planet " + std::to_string(i);
            }

            {
                auto camera = _scene->createEntity();
                auto& component = camera.emplace<Raindrop::Components::Camera>();
                camera.get<Raindrop::Components::Transform>().translation = glm::vec3(0, 1, 0);
            }

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

        struct{
            using Phase = Raindrop::Scene::PhaseID;

            Phase preUpdate;
            Phase update;
            Phase postUpdate;

            Phase preRender;
            Phase render;
            Phase postRender;
            
        } phase;

        std::shared_ptr<Raindrop::Filesystem::FilesystemModule> _filesystem;
        std::shared_ptr<Raindrop::Render::RenderCoreModule> _renderCore;
        std::shared_ptr<Raindrop::Render::RenderOutputModule> _outputs;
        std::shared_ptr<Raindrop::ImGui::ImGuiModule> _imGuiModule;
        std::shared_ptr<Raindrop::ImGui::ImGuiContext> _imGuiCtx;
        std::shared_ptr<Raindrop::Asset::AssetModule> _assets;

        std::shared_ptr<Editor::Editor> _editor;
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