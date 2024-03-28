#ifndef __RAINDROP_RENDERER_BASE_FRAMEBUFFER_RENDER_PASS_HPP__
#define __RAINDROP_RENDERER_BASE_FRAMEBUFFER_RENDER_PASS_HPP__

#include "common.hpp"

namespace Raindrop::Renderer::BaseFramebuffer{
	class RenderPass{
		public:
			RenderPass(Context& context);
			~RenderPass();

			VkRenderPass get() const;

		private:
			Context& _context;

			VkRenderPass _renderPass;

			void createRenderPass();
	};
}

#endif