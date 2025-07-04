#include "Raindrop/Graphics/Backend/Vulkan/Device.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Buffer.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/DeviceConfig.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/Queue.hpp"
#include "Raindrop/Graphics/Backend/Vulkan/WindowContext.hpp"
#include "Raindrop/Window/Config.hpp"
#include "Raindrop/Window/Size.hpp"
#include "Raindrop/Window/SurfaceProviders/Vulkan.hpp"
#include "Raindrop/Window/Window.hpp"
#include "VkBootstrap.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks-inl.h"
#include <vulkan/vulkan.h>
#include <exception>
#include <memory>
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

        createLogger();
        SPDLOG_LOGGER_INFO(_context.logger, "Intializing Vulkan backend...");

        std::shared_ptr<Window::SurfaceProviders::Vulkan> provider = nullptr;
        std::shared_ptr<Window::Window> tempWindow;
        bool buildSwapchain = false;

        // If there is a window manager, the graphics engine will support present
        if (_windowManager){

            // If there a now windows, it creates an empty one to ensure compatility, but will not initialize a swapchain as there is no need
            if (_windowManager->getSize() == 0){
                tempWindow = _windowManager->createWindow(Window::Config::Empty(_gfxEngine.getEngine()));

            } else {
                tempWindow = _windowManager->getWindows().front().lock();
                buildSwapchain = true;
            }
        }

        if (tempWindow){
            provider = tempWindow->getSurfaceProvider<Window::SurfaceProviders::Vulkan>();
        }

        createInstance(provider);
        findPhysicalDevice(provider);
        createDevice();
        getQueues();

        if (buildSwapchain){
            SPDLOG_LOGGER_TRACE(_context.logger, "Building swapchain");

            auto windows = _windowManager->getWindows();

            for (auto weakWindow : windows){
                auto window = weakWindow.lock();

                if (!window) continue;

                createSwapchain(window);
            }
        }
    }

    Device::~Device(){
        vkDeviceWaitIdle(_context.device);

        if (_context.physicalDevice.surface){
            vkb::destroy_surface(_context.instance, _context.physicalDevice.surface);
        }

        vkb::destroy_device(_context.device);
        vkb::destroy_instance(_context.instance);
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
            #ifndef NDEBUG
                .set_debug_callback(&debugCallback)
                .enable_validation_layers()
            #endif
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
            .allow_any_gpu_device_type(false)
            .select();
        
        if (!result){
            SPDLOG_LOGGER_ERROR(_context.logger, "Failed to find physical device : {} ", result.error().message());
            throw std::runtime_error(result.error().message());
        }

        auto device = result.value();

        SPDLOG_LOGGER_INFO(_context.logger, "Physical device found: {} ", device.name);
        _context.physicalDevice = device;
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

    void Device::createSwapchain(std::shared_ptr<Window::Window>){
        
    }

    void Device::getQueues(){
        auto& device = _context.device;
        auto graphics = device.get_queue_index(vkb::QueueType::graphics);
        device.get_queue(vkb::QueueType::graphics);

        // auto initQueue = [device](const vkb::QueueType& type) -> Queue::Queue {
		// 	// We can skip queue check because we already did that
		// 	return Queue::Queue{ 
        //             .family = device.get_queue_index(type).value(),
		// 			.queue = device.get_queue(type).value()
		// 		};
		// };

		// _context.graphics = initQueue(vkb::QueueType::graphics);
		// _context.present = initQueue(vkb::QueueType::present);
		// _context.transfer = initQueue(vkb::QueueType::transfer);
	}
    
    std::shared_ptr<Backend::Buffer> Device::createBuffer(const Buffer::Description& description){
        std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(_context, description);
        return std::static_pointer_cast<Backend::Buffer>(buffer);
    }

    std::shared_ptr<Backend::CommandList> Device::createCommandList(){
        return nullptr;
    }

    std::shared_ptr<Backend::DescriptorSet> Device::createDescriptorSet(){
        return nullptr;
    }

    std::shared_ptr<Backend::Framebuffer> Device::createFramebuffer(){
        return nullptr;
    }

    std::shared_ptr<Backend::GraphicsPipeline> Device::createGraphicsPipeline(){
        return nullptr;
    }

    std::shared_ptr<Backend::RenderPass> Device::createRenderPass(){
        return nullptr;
    }

    std::shared_ptr<Backend::Shader> Device::createShader(){
        return nullptr;
    }
}