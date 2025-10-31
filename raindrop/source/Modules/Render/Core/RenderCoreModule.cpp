#include <vulkan/vulkan.h>

// #define VMA_STATIC_VULKAN_FUNCTIONS 1
// #define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
// #define VMA_VULKAN_VERSION 1003000

#define VMA_IMPLEMENTATION 1

#include "Raindrop/Modules/Render/Core/RenderCoreModule.hpp"
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
    class RenderCoreErrorCategory : public std::error_category{
        public:
            const char* name() const noexcept override{
                return "Render Core";
            }

            std::string message(int ev) const override{
                using ErrorCode = RenderCoreModule::ErrorCode;

                switch (static_cast<ErrorCode>(ev)){
                    case ErrorCode::FAILED_INSTANCE_CREATION: return "Failed instance creation";
                    case ErrorCode::MISSING_INSTANCE_EXTENSIONS: return "Missing instance extension";
                    case ErrorCode::MISSING_LAYER: return "Missing layer";
                    case ErrorCode::FAILED_SURFACE_CREATION: return "Failed surface creation";
                    case ErrorCode::NO_PRESENT_SUPPORT: return "No present support";
                    case ErrorCode::NO_SUITABLE_PHYSICAL_DEVICE: return "No suitable physical device";
                    case ErrorCode::NO_COMPATIBLE_QUEUE_FAMILY: return "No compatible queue family";
                    case ErrorCode::FAILED_LOGICAL_DEVICE_CREATION: return "Failed logical device creation";
                    case ErrorCode::FAILED_ALLOCATOR_CREATION: return "Failed allocator creation";
                    default: return "Unknown system error";
                }
            }
    };

    std::error_category& RenderCoreModule::error_category(){
        static RenderCoreErrorCategory category;
        return category;
    }

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

        if (_allocator){
            vmaDestroyAllocator(_allocator);
        }

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
            .and_then([this, &init]{return createDevice(init);})
            .and_then([this] {return findQueues();})
            .and_then([this]{return createVmaAllocator();});
        
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

    std::expected<void, Error> RenderCoreModule::createInstance(InitData& init){
        vkb::InstanceBuilder builder;

        if (init.window){
            auto extensions = init.window->getInstanceExtensions();
            for (const auto& ext : extensions){
                builder.enable_extension(ext.data());
            }
        }

        auto result = builder
            .set_engine_name("Raindrop")
            .set_minimum_instance_version(VK_API_VERSION_1_4)
            .set_engine_version(VK_API_VERSION_1_4)
            .set_app_version(VK_API_VERSION_1_4)
            .require_api_version(1, 4, 0)
            #ifndef NDEBUG
                .set_debug_callback(&debugCallback)
                .enable_validation_layers()
            #endif
            .build();

        if (!result){
            const auto& error = result.error();
            spdlog::error("Failed to create vulkan instance : {}", error.message());
            return std::unexpected(Error(FailedInstanceCreationError(), "Failed to create vulkan instance : {}", error.message()));
        }

        _instance = *result;
        _vkInstance = _instance;

        return {};
    }

    std::expected<void, Error> RenderCoreModule::findPhysicalDevice(InitData& init){
        vkb::PhysicalDeviceSelector selector(_instance);
        
        if (init.window){
            spdlog::trace("Creating window surface...");
            auto result = init.window->createSurface(_vkInstance);

            if (!result){
                auto& error = result.error();
                spdlog::error("Failed to create window surface {} : {} ", error.message(), error.reason());
                return std::unexpected(Error(FailedSurfaceCreationError(), "Failed to create window surface : {}, {}", error.message(), error.reason()));
            }

            init.surface = *result;
            selector.set_surface(init.surface);
        }

        auto result = selector
            .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
            .allow_any_gpu_device_type(false)
            .set_minimum_version(1, 3)
            .select();
        
        if (!result){
            const auto& error = result.error();
            spdlog::error("Failed to find physical device : {} ", error.message());
            return std::unexpected(Error(NoSuitablePhysicalDeviceError(), "Failed to find physical device : {}", error.message()));
        }

        auto device = result.value();

        spdlog::info("Physical device found: {} ", device.name);

        _physicalDevice = device;
        _vkPhysicalDevice = _physicalDevice;

        return {};
    }

    std::expected<void, Error> RenderCoreModule::createDevice(InitData&){
        vkb::DeviceBuilder builder(_physicalDevice);

        auto result = builder.build();

        if (!result){
            const auto& error = result.error();
            spdlog::error("Failed to create logical device : {}", error.message());
            return std::unexpected(Error(FailedLogicalDeviceCreationError(), "Failed to create logical device : {}", error.message()));
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

    std::expected<void, Error> RenderCoreModule::findQueues(){
        auto getQueue = [this] (Queue& queue, vkb::QueueType type) -> std::expected<void, Error> {
            auto vkbQueue = _device.get_queue(type);
            auto index = _device.get_queue_index(type);

            if (!vkbQueue || !index){
                const auto& error = vkbQueue.error();

                std::string str_type;
                switch (type){
                    case vkb::QueueType::present: str_type="present"; break; 
                    case vkb::QueueType::compute: str_type="compute"; break; 
                    case vkb::QueueType::graphics: str_type="graphics"; break; 
                    case vkb::QueueType::transfer: str_type="transfer"; break; 
                }

                return std::unexpected(Error(NoCompatibleQueueFamilyError(), "Failed to find \"{}\" queue family : {}", str_type, error.message()));
            }

            vk::QueueFlags flags;
            switch (type){
                case vkb::QueueType::compute: flags=vk::QueueFlagBits::eCompute; break; 
                case vkb::QueueType::graphics: flags=vk::QueueFlagBits::eGraphics; break; 
                case vkb::QueueType::transfer: flags=vk::QueueFlagBits::eTransfer; break;
                default: break; 
            }

            queue = Queue(_vkDevice, vkbQueue.value(), flags, index.value());
            return {};
        };

        auto result = getQueue(_computeQueue, vkb::QueueType::compute)
            .and_then([this, getQueue]{return getQueue(_graphicsQueue, vkb::QueueType::graphics);})
            .and_then([this, getQueue]{return getQueue(_presentQueue, vkb::QueueType::present);})
            .and_then([this, getQueue]{return getQueue(_transferQueue, vkb::QueueType::transfer);});

        if (!result){
            return std::unexpected(result.error());
        }
        return {};
    }

    std::expected<void, Error> RenderCoreModule::createVmaAllocator(){

        VmaVulkanFunctions vulkanFuncs{};
        vulkanFuncs.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
        vulkanFuncs.vkGetDeviceProcAddr   = vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo info{
            {},
            static_cast<VkPhysicalDevice>(_vkPhysicalDevice),
            static_cast<VkDevice>(_vkDevice),
            0, // default size
            nullptr,
            nullptr,
            nullptr,
            &vulkanFuncs,
            static_cast<VkInstance>(_vkInstance),
            VK_API_VERSION_1_4,
            nullptr
        };

        if (auto result = vmaCreateAllocator(&info, &_allocator); result != VK_SUCCESS){
            return std::unexpected(Error(FailedAllocatorCreationError(), "Failed to create VMA Allocator : ", vk::to_string(vk::Result(result))));
        }

        return {};
    }
}