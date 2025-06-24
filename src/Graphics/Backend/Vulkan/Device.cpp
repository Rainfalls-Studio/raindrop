#include "Raindrop/Graphics/Backend/Vulkan/Device.hpp"
#include "VkBootstrap.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <stdexcept>

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
    Device::Device(Engine& engine){
        createLogger();
        createInstance();
        findPhysicalDevice();
        createDevice();

        SPDLOG_LOGGER_INFO(_context.logger, "Intializing Vulkan backend...");
    }

    void Device::createLogger(){
        _context.logger = spdlog::stdout_color_mt("Raindrop::Graphics::Backend::Vulkan");
    }
    
    void Device::createInstance(){
        vkb::InstanceBuilder builder;

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

    void Device::findPhysicalDevice(){
        vkb::PhysicalDeviceSelector selector(_context.instance);

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
}