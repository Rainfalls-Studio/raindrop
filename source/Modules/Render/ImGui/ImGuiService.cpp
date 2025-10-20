#include "Raindrop/Modules/Render/ImGui/ImGuiService.hpp"
#include "Raindrop/Modules/Render/RenderOutput/WindowRenderOutput.hpp"
#include "Raindrop/Modules/Render/ImGui/backend/imgui_impl_sdl3.h"
#include <imgui_impl_vulkan.h>

namespace Raindrop::Render{
    ImGuiService::ImGuiService(std::shared_ptr<RenderCoreModule> core, std::shared_ptr<spdlog::logger> logger) :
        _core{core},
        _logger{logger}
    {
        IMGUI_CHECKVERSION();
    }

    ImGuiService::~ImGuiService(){
        for (auto& it : _outputToContext){
            deleteContext(*it.second);
        }
        _outputToContext.clear();
    }

    ImGuiService::Context& ImGuiService::getOrCreateContext(const IRenderOutput::Name& name, SharedRenderOutput output){
        {
            auto it = _outputToContext.find(name);

            if (it != _outputToContext.end()){
                auto& context = it->second;
                assert(context && "The context should be valid here");

                
                if (context->output.lock() != output){
                    SPDLOG_LOGGER_INFO(_logger, "There is a missmatch between the previous render output and the current one");
                    context->output = output;
                }

                return *context;
            }
        }

        auto c = std::make_unique<Context>();
        c->output = output;
        c->outputName = name;

        ImGuiContext* previousContext = ImGui::GetCurrentContext();

        c->ctx = ImGui::CreateContext();
        ImGui::SetCurrentContext(c->ctx);

        try{
            setupBackend(*c, output);
        } catch (const std::exception& e){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to setup ImGui backend for render target \"{}\" : ", name, e.what());
            ImGui::DestroyContext(c->ctx);
            ImGui::SetCurrentContext(previousContext);
            throw std::runtime_error("Failed to setup ImGui context");
        }

        c->last = Raindrop::Time::now();
        auto [it, pushed] = _outputToContext.emplace(std::make_pair(IRenderOutput::Name(name), std::move(c)));

        return *it->second;
    }

    void ImGuiService::destroyContext(const IRenderOutput::Name& name){
        SPDLOG_LOGGER_INFO(_logger, "Destroying \"{}\" ImGui context ...", name);
        auto it = _outputToContext.find(name);
        if (it == _outputToContext.end()){
            SPDLOG_LOGGER_WARN(_logger, "The context was already destroyed");
            return;
        }

        deleteContext(*it->second);
        _outputToContext.erase(it);
    }

    void ImGuiService::deleteContext(Context& context){

        ImGui::SetCurrentContext(context.ctx);
        ImGui_ImplVulkan_DestroyFontsTexture();
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();

        destroyDescriptorPool(context.pool);
    }

    vk::DescriptorPool ImGuiService::createDescriptorPool(){
        auto device = _core->device();

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
            SPDLOG_LOGGER_ERROR(_logger, "Failed to create descriptor pool : {}", vk::to_string(result.result));
            return nullptr;
        }
        
        return result.value;
    }

    void ImGuiService::destroyDescriptorPool(vk::DescriptorPool pool){
        auto device = _core->device();

        device.destroyDescriptorPool(pool);
    }

    void ImGuiService::setupBackend(Context& context, const SharedRenderOutput& output){

        auto windowOutput = std::dynamic_pointer_cast<WindowRenderOutput>(output);
        
        if (!windowOutput){
            SPDLOG_LOGGER_ERROR(_logger, "The render output is not a window ! Other form of render output are not supported !");
            throw std::runtime_error("Only window render outputs are supported for now !");
        }

        auto window = windowOutput->window().lock();

        if (!window){
            SPDLOG_LOGGER_ERROR(_logger, "The window is not valid !");
            throw std::runtime_error("The window is not valid !");
        }

        vk::RenderPass renderPass = windowOutput->renderPass();
        if (!renderPass){
            SPDLOG_LOGGER_ERROR(_logger, "The output target doesn't have a render pass !");
            throw std::runtime_error("Missing render pass");
        }

        if (!ImGui_ImplSDL3_InitForVulkan(window->getWindow())){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to initialize SDL3 ImGui implementation : {}", SDL_GetError());
            throw std::runtime_error("Failed to initialize SDL3 implementation");
        }

        VkDescriptorPool pool = createDescriptorPool();
        context.pool = pool;

        if (!pool){
            ImGui_ImplSDL3_Shutdown();
            throw std::runtime_error("Failed to create descriptor pool");
        }

        ImGui_ImplVulkan_InitInfo info{};
        
        info.Device = _core->device();
        info.PhysicalDevice = _core->physicalDevice();
        info.Instance = _core->instance();
        
        auto graphicsQueue = _core->graphicsQueue();
        info.Queue = *graphicsQueue;
        info.QueueFamily = graphicsQueue.familyIndex();

        info.DescriptorPool = pool;

        info.RenderPass = output->renderPass();
        info.ImageCount = windowOutput->getBufferCount();
        info.MinImageCount = 2;
        info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        // info.UseDynamicRendering = true;

        if (!ImGui_ImplVulkan_Init(&info)){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to initialize Vulkan ImGui implementation !");

            destroyDescriptorPool(pool);
            ImGui_ImplSDL3_Shutdown();

            throw std::runtime_error("Failed to initialize Vulkan implementation");
        }

        if (!ImGui_ImplVulkan_CreateFontsTexture()){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to create vulkan fonts textures");

            destroyDescriptorPool(pool);
            ImGui_ImplSDL3_Shutdown();
            ImGui_ImplVulkan_Shutdown();

            throw std::runtime_error("Failed to create Vulkan fonts textures");
        }
    }

    void ImGuiService::begin(Context& context){
        ImGui::SetCurrentContext(context.ctx);

        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiService::end(Context& context){
        ImGui::SetCurrentContext(context.ctx);

        ImGui::EndFrame();
        ImGui::Render();
        
    }
}