#ifndef __RAINDROP_RENDERER_RENDERER_HPP__
#define __RAINDROP_RENDERER_RENDERER_HPP__

#include "common.hpp"

namespace Raindrop::Renderer{
	class Renderer{
		public:
			Renderer(::Raindrop::Context& context);
			~Renderer();

			void render();
			void events();

		private:
			Context* _context;

			VkCommandBuffer beginFrame();
			void endFrame();

			void createRenderCommandPool();
			void allocateFrameCommandBuffers();

			void freeFrameCommandBuffers();
			void destroyRenderCommandPool();

			VkCommandPool _renderCommandPool;
			std::vector<VkCommandBuffer> _frameCommandBuffers;
			std::size_t _currentFrameID;
	};
}

#endif