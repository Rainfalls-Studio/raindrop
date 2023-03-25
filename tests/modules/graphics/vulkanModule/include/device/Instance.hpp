#ifndef __VULKANMODULE_INSTANCE_HPP__
#define __VULKANMODULE_INSTANCE_HPP__

#include "core.hpp"
#include "Allocator.hpp"

class Instance{
	public:
		Instance(GfxAllocator& allocator, bool validation);
		~Instance();

		bool loaded() const;
		bool validation() const;
		VkInstance get() const;

		Array<const char*>& validationLayers();

	private:
		GfxAllocator* _allocator;
		VkInstance _instance;
		VkDebugUtilsMessengerEXT _debugMessenger;

		Array<const char*> _validationLayers;
		Array<const char*> _requiredExtensions;

		bool _validation;
		bool _loaded;

		void initApplicationInfo(VkApplicationInfo& info);
		void initValidationLayers();
		void initRequiredExtentions();
		bool checkRequiredExtentions();

};

#endif