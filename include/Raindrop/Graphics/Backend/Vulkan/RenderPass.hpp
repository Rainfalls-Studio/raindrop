#pragma once

#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
	struct Context;

	class RenderPass{
		public:
			

			RenderPass(Context& context, VkRenderPass renderPass) noexcept;
			~RenderPass();

			RenderPass(const RenderPass&) = delete;
			RenderPass& operator=(const RenderPass&) = delete;

			RenderPass(RenderPass&& other);
			RenderPass& operator=(RenderPass&& other);

			const VkRenderPass& get() const noexcept;

			friend void swap(RenderPass& A, RenderPass& B);

		private:
			Context* _context;
			VkRenderPass _renderPass;
	};
}