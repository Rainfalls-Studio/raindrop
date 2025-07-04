#pragma once

#include "../RenderPass.hpp"
#include <vulkan/vulkan.h>

namespace Raindrop::Graphics::Backend::Vulkan{
	struct Context;

	class RenderPass : public Backend::RenderPass{
		public:
			RenderPass(Context& context, const Description& description);
			virtual ~RenderPass() override;

			const VkRenderPass& get() const noexcept;

            virtual void* getHandle() const noexcept override;
            virtual API getAPI() const noexcept override;

		private:
			Context& _context;
			VkRenderPass _renderPass;
	};
}