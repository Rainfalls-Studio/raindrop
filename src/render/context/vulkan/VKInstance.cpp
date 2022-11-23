#include "core.hpp"
#include "render/context/vulkan/Vulkan_Context.hpp"
#include "render/context/vulkan/Instance.hpp"
#include <vulkan/vk_enum_string_helper.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
	PROFILE_FUNCTION();
	switch (messageSeverity){
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: RND_WARN(pCallbackData->pMessage); break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: RND_ERR(pCallbackData->pMessage); break;
		default:{
			if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT){
				RND_LOG(pCallbackData->pMessage);
			}
			break;
		}
	}

	return VK_FALSE;
}

static inline VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
	PROFILE_FUNCTION();
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator){
	PROFILE_FUNCTION();

	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

namespace rnd::render::context::vulkan{
	void Instance::init(Vulkan_Context* context){
		PROFILE_FUNCTION();
		this->context = context;

		VkApplicationInfo appInfo = getApplicationInfo();
		VkInstanceCreateInfo createInfo = getInstanceCreateInfo(context);

		createInfo.pApplicationInfo = &appInfo;
		
		VkResult result = vkCreateInstance(&createInfo, &context->allocCallback.get(), &instance);

		if (result != VK_SUCCESS){
			RND_RUNTIME_ERR("vkCreateInstance failed : ", string_VkResult(result));
		}

		checkExtensions();
		setupDebugMessenger();
		context->createSurface(instance, surface);
	}

	void Instance::shutdown(){
		PROFILE_FUNCTION();
		if (context->validationlayers){
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, &context->allocCallback.get());
		}

		vkDestroySurfaceKHR(instance, surface, &context->allocCallback.get());
		vkDestroyInstance(instance, &context->allocCallback.get());
	}

	VkApplicationInfo Instance::getApplicationInfo(){
		PROFILE_FUNCTION();
		VkApplicationInfo info{};
		info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		info.apiVersion = VK_API_VERSION_1_2;
		info.pApplicationName = nullptr;
		info.applicationVersion = VK_MAKE_API_VERSION(0, RND_MAJOR, RND_MINOR, RND_PATCH);
		info.pEngineName = nullptr;
		info.engineVersion = VK_MAKE_API_VERSION(0, RND_MAJOR, RND_MINOR, RND_PATCH);
		return info;
	}

	VkInstanceCreateInfo Instance::getInstanceCreateInfo(Vulkan_Context* context){
		PROFILE_FUNCTION();
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

		auto extension = context->getWindowExtensions();
		createInfo.enabledExtensionCount = (uint32_t)extension.size();
		createInfo.ppEnabledExtensionNames = extension.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (context->validationlayers){
			createInfo.enabledExtensionCount = (uint32_t)validationLayers.size();
			createInfo.ppEnabledLayerNames = validationLayers.data();

			setDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		} else {	
			createInfo.enabledExtensionCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;
		}
	}

	void Instance::setDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo){
		PROFILE_FUNCTION();
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
	}

	void Instance::setupDebugMessenger(){
		PROFILE_FUNCTION();
		if (!context->validationlayers) return;
		
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		setDebugMessengerCreateInfo(createInfo);

		VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);

		if (result != VK_SUCCESS){
			RND_RUNTIME_ERR("failed to create a debug messenger for a vulkan context : ", string_VkResult(result));
		}
	}

	void Instance::checkExtensions(){
		PROFILE_FUNCTION();

		uint32_t extensionsCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

		DynamicArray<VkExtensionProperties> extensions(extensionsCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensions.data());

		UnorderedSet<std::string> available;
		for (const auto &extension : extensions){
			available.insert(extension.extensionName);
		}

		auto requiredExtensions = context->getWindowExtensions();

		for (const auto &required : requiredExtensions){
			if (available.find(required) == available.end()){
				RND_RUNTIME_ERR("The vulkan instance is missing an extensions required with SDL2");
			}
		}
	}
}