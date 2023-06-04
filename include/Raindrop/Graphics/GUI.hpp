#ifndef __RAINDROP_GRAPHICS_UI_HPP__
#define __RAINDROP_GRAPHICS_UI_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class GUI{
		public:
			GUI(Renderer& renderer);
			~GUI();

			void newFrame();
			void render(VkCommandBuffer commandBuffer);

		private:
			VkDescriptorPool _imguiPool;
			VkDevice _device;

	};
}

#endif