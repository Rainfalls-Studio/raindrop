#include "Raindrop/Graphics/Backend/Vulkan/Device.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/DeviceConfig.hpp"
#include "Raindrop/Window/Config.hpp"
#include "Raindrop/Window/SurfaceProviders/Vulkan.hpp"
#include "Raindrop/Window/Window.hpp"
#include "VkBootstrap.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include "vulkan/vulkan_core.h"
#include <stdexcept>
#include <span>
#include <vector>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void*){
	static std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("Vulkan");
	
	spdlog::level::level_enum level = spdlog::level::info;

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) level = spdlog::level::trace;
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) level = spdlog::level::info;
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) level = spdlog::level::warn;
	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) level = spdlog::level::err;

	logger->log(level, "VULKAN {} :: {}", pCallbackData->pMessageIdName, pCallbackData->pMessage);
	return VK_FALSE;
}

namespace Raindrop::Graphics::Backend::Vulkan{
    Device::Device(const DeviceConfig& config) : 
            _gfxEngine{config._gfxEngine},
            _assetManager{config._assetManager},
            _windowManager{config._windowManager}
        {

        std::shared_ptr<Window::SurfaceProviders::Vulkan> provider = nullptr;
        std::shared_ptr<Window::Window> _window;
        bool buildSwapchain = false;

        // If there is a window manager, the graphics engine will support present
        if (_windowManager){

            // If there a now windows, it creates an empty one to ensure compatility, but will not initialize a swapchain as there is no need
            if (_windowManager->getSize() == 0){
                _window = _windowManager->createWindow(Window::Config::Empty(_gfxEngine.getEngine()));

            } else {
                _window = _windowManager->getWindows().front().lock();
                buildSwapchain = true;
            }
        }

        if (_window){
            provider = _window->getSurfaceProvider<Window::SurfaceProviders::Vulkan>();
        }

        createLogger();
        createInstance(provider);
        findPhysicalDevice(provider);
        createDevice();

        if (buildSwapchain){
            SPDLOG_LOGGER_TRACE(_context.logger, "Building swapchain");
        }

        SPDLOG_LOGGER_INFO(_context.logger, "Intializing Vulkan backend...");
    }

    Device::~Device(){

    }

    void Device::createLogger(){
        _context.logger = spdlog::stdout_color_mt("Raindrop::Graphics::Backend::Vulkan");
    }
    
    void Device::createInstance(std::shared_ptr<Window::SurfaceProviders::Vulkan> provider){
        vkb::InstanceBuilder builder;

        // If there is a provider, enable the extensions
        if (provider){
            uint32_t count;
            auto extensions = provider->getInstanceExtensions(&count);

            for (uint32_t i=0; i<count; i++){
                builder.enable_extension(extensions[i]);
            }
        }
    
        auto result = builder
            .set_engine_name("Raindrop")
            .set_debug_callback(&debugCallback)
            .build();

        if (!result){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to create Vulkan instance : {} ", result.error().message());
            throw std::runtime_error(result.error().message());
        }

        _context.instance = result.value();
    }

    void Device::findPhysicalDevice(std::shared_ptr<Window::SurfaceProviders::Vulkan> surfaceProvider){
        vkb::PhysicalDeviceSelector selector(_context.instance);

        VkSurfaceKHR surface = VK_NULL_HANDLE;
        
        if (surfaceProvider){
            if (!surfaceProvider->createSurface(_context.instance, nullptr, &surface)){
                SPDLOG_LOGGER_ERROR(_context.logger, "Failed to create surface");
                throw std::runtime_error("Failed to create surface");
            }
            
            selector.set_surface(surface);
        }

        auto result = selector
            .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
            .select_devices();
        
        if (!result){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to find physical device : {} ", result.error().message());
            throw std::runtime_error(result.error().message());
        }

        auto devices = result.value();

        if (devices.empty()){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to find any physical device");
            throw std::runtime_error("Failed to find any physical device");
        }

        SPDLOG_LOGGER_TRACE(_context.logger, "Found {} devices. Picking first one", devices.size());
        for (auto device : devices){
            SPDLOG_LOGGER_TRACE(_context.logger, "\t - {}", device.name);
        }

        _context.physicalDevice = devices[0];

        if (surface != VK_NULL_HANDLE){
            surfaceProvider->destroySurface(_context.instance, surface, nullptr);
        }
    }

    void Device::createDevice(){
        vkb::DeviceBuilder builder(_context.physicalDevice);

        auto result = builder.build();

        if (!result){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to create Vulkan device : {} ", result.error().message());
            throw std::runtime_error(result.error().message());
        }

        _context.device = result.value();
    }

    
    std::shared_ptr<Buffer> Device::createBuffer(){
        return nullptr;
    }

    std::shared_ptr<CommandList> Device::createCommandList(){
        return nullptr;
    }

    std::shared_ptr<DescriptorSet> Device::createDescriptorSet(){
        return nullptr;
    }

    std::shared_ptr<Framebuffer> Device::createFramebuffer(){
        return nullptr;
    }

    std::shared_ptr<GraphicsPipeline> Device::createGraphicsPipeline(){
        return nullptr;
    }

    std::shared_ptr<RenderPass> Device::createRenderPass(){
        return nullptr;
    }

    std::shared_ptr<Sampler> Device::createSampler(){
        return nullptr;
    }

    std::shared_ptr<Shader> Device::createShader(){
        return nullptr;
    }

    std::shared_ptr<Texture> Device::createTexture(){
        return nullptr;
    }
}