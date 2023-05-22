#ifndef __RAINDROP_GRAPHICS_INSTANCE_HPP__
#define __RAINDROP_GRAPHICS_INSTANCE_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class Instance{
		public:
			Instance(VkInstanceCreateInfo& info, VkAllocationCallbacks* callbacks);
			Instance(VkInstance instance);
			~Instance();

			Instance(const Instance&) = delete;
			Instance& operator=(const Instance&) = delete;

			VkInstance get() const;

		private:
			VkAllocationCallbacks* _allocatorCallbacks = nullptr;
			VkInstance _instance = VK_NULL_HANDLE;
	};
}

#endif