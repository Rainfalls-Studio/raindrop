#include <vulkan/vulkan.h>

// #define VMA_STATIC_VULKAN_FUNCTIONS 1
// #define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
// #define VMA_VULKAN_VERSION 1003000

#define VMA_IMPLEMENTATION 1

#include "RenderCore/RenderCoreModule.hpp"
#include <Raindrop/Modules/InitHelper.hpp>

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

extern "C" RAINDROP_EXPORT Raindrop::Modules::IModule* CreateModule(){
	return new Raindrop::Render::RenderCoreModule();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::Modules::IModule* module){
	delete module;
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
        _windowModule = std::dynamic_pointer_cast<Window::WindowModule>(helper.dependencies().at("Window"));

        vk::SurfaceKHR surface = VK_NULL_HANDLE;
        std::shared_ptr<Window::Window> window;

        if (_windowModule){
            window = _windowModule->createWindow(Window::WindowConfig::Empty());
        }

        auto result = _deviceManager.initialize(surface, window)
            .and_then([&]{return findQueues();})
            .and_then([&]{return createVmaAllocator();});

        if (surface){
            _deviceManager.instance().destroySurfaceKHR(surface);
        }

        if (!result){
            const auto& error = result.error();
            return Modules::Result::Error(error.message() + " " + error.reason());
        }

        return Modules::Result::Success();
    }

    DeviceManager& RenderCoreModule::deviceManager() noexcept{
        return _deviceManager;
    }


    void RenderCoreModule::shutdown(){
        if (_allocator){
            vmaDestroyAllocator(_allocator);
        }
        
        _deviceManager.shutdown();
    }

    std::expected<void, Error> RenderCoreModule::findQueues(){
        auto device = _deviceManager.vkbDevice();

        auto getQueue = [&] (Queue& queue, vkb::QueueType type) -> std::expected<void, Error> {
            auto vkbQueue = device.get_queue(type);
            auto index = device.get_queue_index(type);

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

            queue = Queue(device.device, vkbQueue.value(), flags, index.value());
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
            static_cast<VkPhysicalDevice>(_deviceManager.physicalDevice()),
            static_cast<VkDevice>(_deviceManager.device()),
            0, // default size
            nullptr,
            nullptr,
            nullptr,
            &vulkanFuncs,
            static_cast<VkInstance>(_deviceManager.instance()),
            VK_API_VERSION_1_4,
            nullptr
        };

        if (auto result = vmaCreateAllocator(&info, &_allocator); result != VK_SUCCESS){
            return std::unexpected(Error(FailedAllocatorCreationError(), "Failed to create VMA Allocator : ", vk::to_string(vk::Result(result))));
        }

        return {};
    }
}