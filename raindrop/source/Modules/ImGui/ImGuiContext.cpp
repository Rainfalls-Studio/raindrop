#include "Raindrop/Modules/ImGui/ImGuiContext.hpp"
#include "Raindrop/Modules/Render/RenderOutput/WindowRenderOutput.hpp"
#include "Raindrop/Modules/ImGui/backend/imgui_impl_sdl3.h"
#include <imgui_impl_vulkan.h>
#include <spdlog/spdlog.h>

namespace Raindrop::ImGui{
    ImGuiContext::ImGuiContext(std::shared_ptr<Render::RenderCoreModule> core, std::shared_ptr<Render::IRenderOutput> output) : 
        _core{core},
        _output{output}
    {
        _ctx = ::ImGui::CreateContext();
        ::ImGui::SetCurrentContext(_ctx);

        try{
            setupBackend();
        } catch (const std::exception& e){
            spdlog::error("Failed to setup ImGui backend for render target : {}", e.what());

            ::ImGui::DestroyContext(_ctx);
            ::ImGui::SetCurrentContext(nullptr);

            throw std::runtime_error("Failed to setup ImGui context");
        }
    }

    ImGuiContext::~ImGuiContext(){
        ::ImGui::SetCurrentContext(_ctx);
        ImGui_ImplVulkan_DestroyFontsTexture();
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ::ImGui::DestroyContext();

        destroyDescriptorPool();
    }

    vk::DescriptorPool ImGuiContext::createDescriptorPool(){
        auto device = _core->deviceManager().device();

        vk::DescriptorPoolSize sizes[] = {
            { vk::DescriptorType::eSampler, 1000 },
            { vk::DescriptorType::eCombinedImageSampler, 1000 },
            { vk::DescriptorType::eSampledImage, 1000 },
            { vk::DescriptorType::eStorageImage, 1000 },
            { vk::DescriptorType::eUniformTexelBuffer, 1000 },
            { vk::DescriptorType::eStorageTexelBuffer, 1000 },
            { vk::DescriptorType::eUniformBuffer, 1000 },
            { vk::DescriptorType::eStorageBuffer, 1000 },
            { vk::DescriptorType::eUniformBufferDynamic, 1000 },
            { vk::DescriptorType::eStorageBufferDynamic, 1000 },
            { vk::DescriptorType::eInputAttachment, 1000 }
        };

        vk::DescriptorPoolCreateInfo info{
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
            1000,
            sizeof(sizes) / sizeof(sizes[0]),
            sizes
        };

        auto result = device.createDescriptorPool(info);
        if (result.result != vk::Result::eSuccess){
            spdlog::error("Failed to create descriptor pool : {}", vk::to_string(result.result));
            return nullptr;
        }
        
        return result.value;
    }

    void ImGuiContext::destroyDescriptorPool(){
        auto device = _core->deviceManager().device();

        device.destroyDescriptorPool(_pool);
    }


    void ImGuiContext::setupBackend(){
        auto windowOutput = std::dynamic_pointer_cast<Render::WindowRenderOutput>(_output);
        
        if (!windowOutput){
            spdlog::error("The render output is not a window ! Other form of render output are not supported !");
            throw std::runtime_error("Only window render outputs are supported for now !");
        }

        auto window = windowOutput->window().lock();

        if (!window){
            spdlog::error("The window is not valid !");
            throw std::runtime_error("The window is not valid !");
        }

        vk::RenderPass renderPass = windowOutput->renderPass();
        if (!renderPass){
            spdlog::error("The output target doesn't have a render pass !");
            throw std::runtime_error("Missing render pass");
        }

        if (!ImGui_ImplSDL3_InitForVulkan(window->getWindow())){
            spdlog::error("Failed to initialize SDL3 ImGui implementation : {}", SDL_GetError());
            throw std::runtime_error("Failed to initialize SDL3 implementation");
        }

        _pool = createDescriptorPool();

        if (!_pool){
            ImGui_ImplSDL3_Shutdown();
            throw std::runtime_error("Failed to create descriptor pool");
        }

        ImGui_ImplVulkan_InitInfo info{};

        auto& deviceManager = _core->deviceManager();
        
        info.Device = deviceManager.device();
        info.PhysicalDevice = deviceManager.physicalDevice();
        info.Instance = deviceManager.instance();
        
        auto graphicsQueue = _core->graphicsQueue();
        info.Queue = *graphicsQueue;
        info.QueueFamily = graphicsQueue.familyIndex();

        info.DescriptorPool = _pool;

        info.RenderPass = _output->renderPass();
        info.ImageCount = windowOutput->getBufferCount();
        info.MinImageCount = 2;
        info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        // info.UseDynamicRendering = true;

        if (!ImGui_ImplVulkan_Init(&info)){
            spdlog::error("Failed to initialize Vulkan ImGui implementation !");

            destroyDescriptorPool();
            ImGui_ImplSDL3_Shutdown();

            throw std::runtime_error("Failed to initialize Vulkan implementation");
        }

        if (!ImGui_ImplVulkan_CreateFontsTexture()){
            spdlog::error("Failed to create vulkan fonts textures");

            destroyDescriptorPool();
            ImGui_ImplSDL3_Shutdown();
            ImGui_ImplVulkan_Shutdown();

            throw std::runtime_error("Failed to create Vulkan fonts textures");
        }


        ::ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }


    void ImGuiContext::begin(){
        ::ImGui::SetCurrentContext(_ctx);

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ::ImGui::NewFrame();

        float scale = _output->scale();

        ImGuiStyle& style = ::ImGui::GetStyle();
        ImGuiStyle styleold = style; // Backup colors
        style = ImGuiStyle(); // IMPORTANT: ScaleAllSizes will change the original size, so we should reset all style config
        style.WindowBorderSize = 1.0f;
        style.ChildBorderSize  = 1.0f;
        style.PopupBorderSize  = 1.0f;
        style.FrameBorderSize  = 1.0f;
        style.TabBorderSize    = 1.0f;
        style.WindowRounding    = 0.0f;
        style.ChildRounding     = 0.0f;
        style.PopupRounding     = 0.0f;
        style.FrameRounding     = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding      = 0.0f;
        style.TabRounding       = 0.0f;
        style.ScaleAllSizes(scale);
        std::memcpy(style.Colors, styleold.Colors, sizeof(style.Colors));

        ::ImGui::GetIO().FontGlobalScale = scale;

        ::ImGui::ShowDemoWindow();
    }

    void ImGuiContext::end(){
        ::ImGui::SetCurrentContext(_ctx);
        ::ImGui::EndFrame();
    }

    void ImGuiContext::render(vk::CommandBuffer cmdBuffer){
        ::ImGui::SetCurrentContext(_ctx);

        ::ImGui::Render();
        auto drawData = ::ImGui::GetDrawData();
        ImGui_ImplVulkan_RenderDrawData(drawData, cmdBuffer);
    }



    // ------------------------------------------------------------------------------------




    BeginStage::BeginStage(std::shared_ptr<ImGuiContext> context) : _ctx(context){}

    const char* BeginStage::name() const{
        return "ImGui - Begin Stage";
    }

    void BeginStage::shutdown() {}

    Scheduler::StageResult BeginStage::execute(){
        auto context = _ctx.lock();

        if (!context){
            return Scheduler::StageResult::Skip("No context");
        }

        context->begin();
        return Scheduler::StageResult::Continue();
    }



    // -----------------------------------------------------------------------------------




    EndStage::EndStage(std::shared_ptr<ImGuiContext> context) : _ctx(context){}

    const char* EndStage::name() const{
        return "ImGui - End Stage";
    }

    void EndStage::shutdown() {}

    Scheduler::StageResult EndStage::execute(){
        auto context = _ctx.lock();

        if (!context){
            return Scheduler::StageResult::Skip("No context");
        }

        context->end();
        return Scheduler::StageResult::Continue();
    }



    // -----------------------------------------------------------------------------------

    

    RenderStage::RenderStage(std::shared_ptr<ImGuiContext> context, std::shared_ptr<Render::RenderCommandContext> cmdCtx) : 
        _ctx{context},
        _cmdCtx(cmdCtx)
    {}

    const char* RenderStage::name() const{
        return "ImGui - Render Stage";
    }

    void RenderStage::shutdown(){}
    Scheduler::StageResult RenderStage::execute(){
        auto context = _ctx.lock();

        if (!context){
            return Scheduler::StageResult::Skip("No context");
        }

        auto cmdCtx = _cmdCtx.lock();

        if (!cmdCtx){
            return Scheduler::StageResult::Skip("No render command context");
        }

        vk::CommandBuffer cmdBuffer = cmdCtx->currentBuffer().cmdBuffer;
        context->render(cmdBuffer);

        return Scheduler::StageResult::Continue();
    }
}