#ifndef __RAINDROP_BUILDERS_INSTANCE_BUILDER_HPP__
#define __RAINDROP_BUILDERS_INSTANCE_BUILDER_HPP__

#include <Raindrop/Graphics/common.hpp>
#include <Raindrop/Graphics/Instance.hpp>

namespace Raindrop::Graphics::Builders{
	class InstanceBuilder{
		public:
			InstanceBuilder();
			~InstanceBuilder();

			InstanceBuilder& setApplicationName(const std::string& name);
			InstanceBuilder& setEngineName(const std::string& name);

			InstanceBuilder& setApplicationVersion(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch);
			InstanceBuilder& setEngineVersion(uint32_t variant, uint32_t major, uint32_t minor, uint32_t patch);
			InstanceBuilder& setVulkanAPIVersion(uint32_t version);

			InstanceBuilder& requireLayer(const char* layer);
			InstanceBuilder& requireExtension(const char* extension);

			InstanceBuilder& requireGraphicsExtensions();
			InstanceBuilder& requireValidationLayer(const char* layer);

			std::shared_ptr<Instance> build(VkAllocationCallbacks* callbacks = nullptr);

			bool hasValidation() const;

		private:
			std::string _applicationName;
			std::string _engineName;
			
			uint32_t _applicationVersion = 0;
			uint32_t _engineVersion = 0;
			uint32_t _apiVersion = 0;

			std::vector<const char*> _requiredLayers;
			std::vector<const char*> _requiredExtensions;
			std::vector<const char*> _requiredValidationLayers;

			void checkExtensions();
			void checkLayers();
			void checkValidationLayers();
			void setupDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& info);
	};
}

#endif