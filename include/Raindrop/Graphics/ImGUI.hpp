#ifndef __RAINDROP_GRAPHICS_IMGUI_HPP__
#define __RAINDROP_GRAPHICS_IMGUI_HPP__

#include <Raindrop/Graphics/common.hpp>

namespace Raindrop::Graphics{
	class ImGUI{
		public:
			ImGUI(Renderer& renderer);
			~ImGUI();

			void newFrame();
			void render(VkCommandBuffer commandBuffer);

			void event(SDL_Event* e);

		private:
			VkDescriptorPool _imguiPool;
			VkDevice _device;

	};
}

#endif