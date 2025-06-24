#pragma once

#include <VkBootstrap.h>

namespace Raindrop::Graphics{
	class Context;

	namespace Backend::Vulkan{
		class Instance{
			public:
				Instance(Context& context, vkb::Instance instance) noexcept;
				Instance(Context& context, VkInstance instance) noexcept = delete;
				~Instance();

				Instance(const Instance&) = delete;
				Instance& operator=(const Instance&) = delete;

				vkb::Instance* operator->() noexcept;
				vkb::Instance& operator*() noexcept;

				const VkInstance& get() const noexcept;
				const vkb::Instance& getVkb() const noexcept;

			private:
				Context* _context;
				vkb::Instance _instance;
		};
	}
}