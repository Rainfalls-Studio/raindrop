#include "Raindrop/Modules/Render/RenderCoreModule.hpp"
#include "Raindrop/Core/Modules/InitHelper.hpp"

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


namespace Raindrop::Render{
    RenderCoreModule::RenderCoreModule(){}

    Modules::Result RenderCoreModule::initialize(Modules::InitHelper& helper){
        _engine = &helper.engine();
        _window = std::dynamic_pointer_cast<Window::WindowModule>(helper.dependencies().at("Window"));

        return initVulkan();
    }

    void RenderCoreModule::shutdown(){
        destroyVulkan();
    }

    void RenderCoreModule::destroyVulkan(){
        spdlog::info("Destroying vulkan...");

        if (_device){
            spdlog::trace("Destroying vulkan device...");
            vkDeviceWaitIdle(_device);
            vkb::destroy_device(_device);
        }

        if (_instance){
            spdlog::trace("Destroying vulkan instance...");
            vkb::destroy_instance(_instance);
        }
    }

    Modules::Result RenderCoreModule::initVulkan(){
        spdlog::info("Initializing vulkan...");

        std::shared_ptr<Window::Window> empty;

        if (_window){
            spdlog::trace("Window system available. Creating empty window for surface presentation...");
            empty = _window->createWindow(Window::WindowConfig::Empty());
        }

        InitData init{
            .window = empty,
            .surface = VK_NULL_HANDLE
        };

        auto result =
            createInstance(init)
            .and_then([this, &init]{return findPhysicalDevice(init);})
            .and_then([this, &init]{return createDevice(init);});
        
        if (!result){
            auto& error = result.error();
            spdlog::error("Failed to initialize vulkan : {}", error.message());
            return Modules::Result::Error("Failed to initialize vulkan");
        }

        if (_window){
            vkDestroySurfaceKHR(_instance, init.surface, _instance.allocation_callbacks);
            init.window.reset();
        }

        return Modules::Result::Success();
    }

    std::expected<void, std::error_code> RenderCoreModule::createInstance(InitData& init){
        vkb::InstanceBuilder builder;

        if (init.window){
            auto extensions = init.window->getInstanceExtensions();
            for (const auto& ext : extensions){
                builder.enable_extension(ext.data());
            }
        }

        auto result = builder
            .set_engine_name("Raindrop")
            .set_minimum_instance_version(VK_VERSION_1_2)
            #ifndef NDEBUG
                .set_debug_callback(&debugCallback)
                .enable_validation_layers()
            #endif
            .build();

        if (!result){
            const auto& error = result.error();
            spdlog::error("Failed to create vulkan instance : {}", error.message());
            return std::unexpected(error);
        }

        _instance = *result;
        _vkInstance = _instance;

        return {};
    }

    std::expected<void, std::error_code> RenderCoreModule::findPhysicalDevice(InitData& init){
        vkb::PhysicalDeviceSelector selector(_instance);
        
        if (init.window){
            spdlog::trace("Creating window surface...");
            auto result = init.window->createSurface(_vkInstance);

            if (!result){
                auto& error = result.error();
                spdlog::error("Failed to create window surface {} : {} ", error.message(), error.reason());
                return std::unexpected(error.code());
            }

            init.surface = *result;
            selector.set_surface(init.surface);
        }

        auto result = selector
            .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
            .allow_any_gpu_device_type(false)
            .select();
        
        if (!result){
            spdlog::error("Failed to find physical device : {} ", result.error().message());
            return std::unexpected(result.error());
        }

        auto device = result.value();

        spdlog::info("Physical device found: {} ", device.name);

        _physicalDevice = device;
        _vkPhysicalDevice = _physicalDevice;

        return {};
    }

    std::expected<void, std::error_code> RenderCoreModule::createDevice(InitData&){
        vkb::DeviceBuilder builder(_physicalDevice);

        auto result = builder.build();

        if (!result){
            return std::unexpected(result.error());
        }

        _device = *result;
        _vkDevice = _device;

        return {};
    }

    RenderCoreModule::Name RenderCoreModule::name() const noexcept{
        return "RenderCore";
    }

    Modules::DependencyList RenderCoreModule::dependencies() const noexcept{
        return {
            Modules::Dependency("Window", true)
        };
    }

    Modules::Result RenderCoreModule::dependencyReload(const Name& name){
        if (name == "Window"){
            _window = _engine->getModuleManager().getModuleAs<Window::WindowModule>("Window");
            destroyVulkan();
            return initVulkan();
        }

        return Modules::Result::Success();
    }

    Modules::Result RenderCoreModule::dependencyShutdown(const Name& name){
        if (name == "Window"){
            _window.reset();
            destroyVulkan();
            return initVulkan();
        }

        return Modules::Result::Success();
    }
}