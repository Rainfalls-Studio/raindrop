#include "render/vulkan/Instance.hpp"
#include "core.hpp"

#include <cstring>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
	PROFILE_FUNCTION();

	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT){
		fprintf(stderr, "%s\n", pCallbackData->pMessage);
		
		return VK_FALSE;
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

namespace rnd::render::vulkan{
	void Instance::init(InstanceBuilder &builder){
		PROFILE_FUNCTION();
		validationLayerEnabled = builder.validationLayersEnabled;
		if (validationLayerEnabled){
			checkValidationLayers(builder.validationLayers);
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = builder.applicationName.c_str();
		appInfo.applicationVersion = builder.applicationVersion;
		appInfo.pEngineName = builder.engineName.c_str();
		appInfo.engineVersion = builder.engineVersion;
		appInfo.apiVersion = builder.apiVersion;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions(builder.window);
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (validationLayerEnabled){
			createInfo.enabledLayerCount = static_cast<uint32_t>(builder.validationLayers.size());
			createInfo.ppEnabledLayerNames = builder.validationLayers.data();

			setDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = &debugCreateInfo;
		} else {
			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;
		}
		
		{
			PROFILE_SCOPE("vkCreateInstance");
			VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

			if (result != VK_SUCCESS){
				RND_RUNTIME_ERR("failed to create the vulkan instance : vkCreateInstance :: ", string_VkResult(result));
			}
		}

		checkRequiredExtensions(builder.window);
		setupDebugMessenger();

		{
			PROFILE_SCOPE("SDL_Vulkan_CreateSurface");
			SDL_bool result = SDL_Vulkan_CreateSurface(static_cast<SDL_Window*>(builder.window), instance, &surface);
			if (result == SDL_FALSE){
				RND_RUNTIME_ERR("failed to create the window surface : SDL_Vulkan_CreateSurface :: ", SDL_GetError());
			}
		}

		validationLayers.resize(builder.validationLayers.size());
		for (int i=0; i<static_cast<int>(validationLayers.size()); i++){
			auto &l = validationLayers[i];
			l = new char[strlen(builder.validationLayers[i])];

			strcpy(l, builder.validationLayers[i]);
		}
	}

	void Instance::shutdown(){
		PROFILE_FUNCTION();
		if (validationLayerEnabled) DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

		for (auto &layer : validationLayers){
			delete[] layer;
		}

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);
	}

	void Instance::checkValidationLayers(std::vector<const char *> &validationLayers){
		PROFILE_FUNCTION();
		uint32_t count;
		vkEnumerateInstanceLayerProperties(&count, nullptr);

		std::vector<VkLayerProperties> availableLayers(count);
		vkEnumerateInstanceLayerProperties(&count, availableLayers.data());

		for (auto &layerName : validationLayers){
			bool layerFound = false;

			for (const auto &layerProperties : availableLayers){
				if (strcmp(layerName, layerProperties.layerName) == 0){
					layerFound = true;
					break;
				}
			}

			if (!layerFound){
				RND_RUNTIME_ERR("vulkan instance, failed to found \"" + std::string(layerName) + "\" validation layer");
			}
		}
	}

	std::vector<const char*> Instance::getRequiredExtensions(void* window){
		PROFILE_FUNCTION();
		SDL_Window* win = static_cast<SDL_Window*>(window);

		unsigned int count = 0;
		SDL_Vulkan_GetInstanceExtensions(win, &count, nullptr);

		std::vector<const char*> extensions(count);
		SDL_Vulkan_GetInstanceExtensions(win, &count, extensions.data());

		if (validationLayerEnabled){
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	void Instance::setDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo){
		PROFILE_FUNCTION();
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;
	}

	void Instance::checkRequiredExtensions(void *window){
		PROFILE_FUNCTION();
		uint32_t extensionsCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionsCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensions.data());

		std::unordered_set<std::string> available;
		for (const auto &extension : extensions){
			available.insert(extension.extensionName);
		}

		auto requiredExtensions = getRequiredExtensions(window);

		for (const auto &required : requiredExtensions){
			if (available.find(required) == available.end()){
				RND_RUNTIME_ERR("vulkan instance, missing required SDL2 vulkan extention");
			}
		}
	}

	void Instance::setupDebugMessenger(){
		PROFILE_FUNCTION();
		if (!validationLayerEnabled) return;
		
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		setDebugMessengerCreateInfo(createInfo);

		{
			VkResult result = CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
			if (result != VK_SUCCESS){
				RND_RUNTIME_ERR("vulkan instance, failed to create the debug messenger : CreateDebugUtilsMessengerEXT :: ", string_VkResult(result));
			}
		}
	}

	bool Instance::isValidationLayersEnabled() const{
		PROFILE_FUNCTION();
		return validationLayerEnabled;
	}

	VkInstance Instance::getInstance() const{
		PROFILE_FUNCTION();
		return instance;
	}

	VkSurfaceKHR Instance::getSurface() const{
		PROFILE_FUNCTION();
		return surface;
	}

	std::vector<char*>& Instance::getValidationLayers(){
		PROFILE_FUNCTION();
		return validationLayers;
	}
}