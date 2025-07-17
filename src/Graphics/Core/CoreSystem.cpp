#include "Raindrop/Graphics/Core/CoreSystem.hpp"
#include "Raindrop/Graphics/GraphicsEngine.hpp"
#include "Raindrop/Window/WindowSystem.hpp"
#include <VkBootstrap.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

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

namespace Raindrop::Graphics::Core{
    void CoreSystem::initialize(GraphicsEngine& engine){
        createLogger();

        std::shared_ptr<Window::WindowSystem> system;

        try{
            system = engine.getEngine().getSystemManager().getSystem<Raindrop::Window::WindowSystem>();
        } catch (const std::out_of_range&){ // window system is not registred
            SPDLOG_LOGGER_INFO(_logger, "No windowing manager registred, the graphics engine will be in headless mode");
        } catch (...){
            throw;
        }


        InitData init;
        std::shared_ptr<Window::Window> empty;
        
        // If there no windowing system, render is headless mode
        if (system){
            SPDLOG_LOGGER_TRACE(_logger, "Creating empty window for vulkan initilization");
            empty = system->createWindow(Window::WindowConfig::Empty());
        }

        init.window = empty.get();
        
        SPDLOG_LOGGER_TRACE(_logger, "Initializing vulkan...");
        createInstance(init);
        findPhysicalDevice(init);
        createDevice(init);

        if (init.surface){
            _instance.destroySurfaceKHR(init.surface);
        }
    }

    void CoreSystem::createLogger(){
        _logger = spdlog::get("Raindrop::Graphics::Core");
        if (!_logger){
            _logger = spdlog::stdout_color_mt("Raindrop::Graphics::Core");
        }
    }   

    void CoreSystem::shutdown(){
        _device.waitIdle();
        vkb::destroy_device(_vkbDevice);
        vkb::destroy_instance(_vkbInstance);
    }

    void CoreSystem::createInstance(InitData& init){
        vkb::InstanceBuilder builder;
        
        if (init.window){
            auto extensions = init.window->getInstanceExtensions();
            for (const auto& ext : extensions){
                builder.enable_extension(ext.data());
            }
        }

        auto result = builder
            .set_engine_name("Raindrop")
            #ifndef NDEBUG
                .set_debug_callback(&debugCallback)
                .enable_validation_layers()
            #endif
            .set_minimum_instance_version(VK_VERSION_1_3)
            .build();
        

        if (!result){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to create Vulkan instance : {} ", result.error().message());
            throw std::runtime_error(result.error().message());
        }

        _vkbInstance = result.value();
        _instance = _vkbInstance.instance;
    }

    void CoreSystem::findPhysicalDevice(InitData& init){
        vkb::PhysicalDeviceSelector selector(_vkbInstance);
        
        if (init.window){
            init.surface = init.window->createSurface(_instance);
            selector.set_surface(init.surface);
        }

        auto result = selector
            .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
            .allow_any_gpu_device_type(false)
            .select();
        
        if (!result){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to find physical device : {} ", result.error().message());
            throw std::runtime_error(result.error().message());
        }

        auto device = result.value();

        SPDLOG_LOGGER_INFO(_logger, "Physical device found: {} ", device.name);
        _vkbPhysicalDevice = device;
        _physicalDevice = _vkbPhysicalDevice;
    }

    void CoreSystem::createDevice(InitData&){
        vkb::DeviceBuilder builder(_vkbPhysicalDevice);

        auto result = builder.build();

        if (!result){
            SPDLOG_LOGGER_ERROR(_logger, "Failed to create Vulkan device : {} ", result.error().message());
            throw std::runtime_error(result.error().message());
        }

        _vkbDevice = result.value();
        _device = _vkbDevice.device;
    }

    const char* CoreSystem::name() const{
        return "Core";
    }

    vk::Instance CoreSystem::getInstance() const{
        return _instance;
    }

    vk::PhysicalDevice CoreSystem::getPhysicalDevice() const{
        return _physicalDevice;
    }

    vk::Device CoreSystem::getDevice() const{
        return _device;
    }
}