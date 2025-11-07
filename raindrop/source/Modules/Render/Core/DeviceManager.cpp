#include "Raindrop/Modules/Render/Core/DeviceManager.hpp"


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
    class DeviceManagerErrorCategory : public std::error_category{
        public:
            const char* name() const noexcept override{
                return "Device Manager";
            }

            std::string message(int ev) const override{
                using ErrorCode = DeviceManager::ErrorCode;

                switch (static_cast<ErrorCode>(ev)){
                    case ErrorCode::FAILED_INSTANCE_CREATION: return "Failed instance creation";
                    case ErrorCode::PHYSICAL_DEVICE_NOT_FOUND: return "Failed to find a suitable physical device";
                    case ErrorCode::FAILED_DEVICE_CREATION: return "Failed to create device";
                    default: return "Unknown system error";
                }
            }
    };

    std::error_category& DeviceManager::error_category(){
        static DeviceManagerErrorCategory category;
        return category;
    }

    DeviceManager::DeviceManager(){}
    DeviceManager::~DeviceManager(){}

    std::expected<void, Error> DeviceManager::initialize(vk::SurfaceKHR& surface, std::shared_ptr<Window::Window>& window){
        
        InitData data{
            surface,
            window
        };

        auto result = createInstance(data)
            .and_then([&]{return pickPhysicalDevice(data);})
            .and_then([&]{return createDevice(data);});

        return result;
    }

    void DeviceManager::shutdown(){
        if (_device){
            if (auto result = _vkDevice.waitIdle(); result != vk::Result::eSuccess){
                spdlog::warn("Failed to wait device for idle : {}", vk::to_string(result));
            }

            vkb::destroy_device(_device);
            _vkDevice = VK_NULL_HANDLE;
        }

        if (_instance){
            vkb::destroy_instance(_instance);
            _vkInstance = VK_NULL_HANDLE;
        }
    }


    std::expected<void, Error> DeviceManager::createInstance(InitData& init){
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
            return std::unexpected(Error(FailedInstanceCreationError(), "Failed to create instance : {}", error.message()));
        }

        _instance = *result;
        _vkInstance = _instance;

        return {};
    }

    std::expected<void, Error> DeviceManager::pickPhysicalDevice(InitData& init){
        vkb::PhysicalDeviceSelector selector(_instance);
        
        if (init.window){
            auto result = init.window->createSurface(_vkInstance);

            if (!result){
                auto& error = result.error();
                return std::unexpected(error);
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
            return std::unexpected(Error(PhysicalDeviceNotFoundError(), "Failed to find physical device : {}", error.message()));
        }

        auto device = result.value();

        spdlog::info("Physical device found: {} ", device.name);

        _physicalDevice = device;
        _vkPhysicalDevice = _physicalDevice;

        return {};
    }

    std::expected<void, Error> DeviceManager::createDevice(InitData& ){
        vkb::DeviceBuilder builder(_physicalDevice);

        auto result = builder.build();

        if (!result){
            const auto& error = result.error();
            return std::unexpected(Error(FailedDeviceCreationError(), "Failed to create device : {}", error.message()));
        }

        _device = *result;
        _vkDevice = _device;

        return {};
    }


    vk::Device DeviceManager::device() const{
        return _vkDevice;
    }

    vk::PhysicalDevice DeviceManager::physicalDevice() const{
        return _vkPhysicalDevice;
    }

    vk::Instance DeviceManager::instance() const{
        return _vkInstance;
    }

    vkb::Instance DeviceManager::vkbInstance() const{
        return _instance;
    }

    vkb::PhysicalDevice DeviceManager::vkbPhysicalDevice() const{
        return _physicalDevice;
    }

    vkb::Device DeviceManager::vkbDevice() const{
        return _device;
    }

}