#pragma once

#include <vulkan/vulkan.h>
#include "../enum.hpp"
#include <string>
#include <vector>

namespace rnd::render::vulkan{
	class InstanceBuilder{
		friend class Instance;
		public:
			void setApplicationName(const char* name);
			void setEngineName(const char* name);
			void setApplicationVersion(uint16_t major, uint16_t minor, uint16_t patch);
			void setEngineVersion(uint16_t major, uint16_t minor, uint16_t patch);
			void setVulkanVersion(VulkanVersion&& version);
			void requireExtension(const char* extension);
			void requireValidationLayer(const char* layer);
			void enableValidationLayers(bool enabled = true);
			void setWindow(void* window);
		
		private:
			std::string applicationName = "NoNameApp";
			std::string engineName = "render/vulkanEngine";
			uint32_t applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			uint32_t engineVersion = VK_MAKE_VERSION(1, 0, 0);
			uint32_t apiVersion = VK_API_VERSION_1_1;
			std::vector<std::string> requiredExtensions;
			std::vector<const char*> validationLayers;
			bool validationLayersEnabled = false;
			void* window = nullptr;
	};
}