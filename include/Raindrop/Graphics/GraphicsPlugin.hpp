#ifndef __RAINDROP_GRAPHICS_PLUGIN_HPP__
#define __RAINDROP_GRAPHICS_PLUGIN_HPP__

#include <Graphics/common.hpp>

namespace Raindrop::Graphics{
	class RAINDROP_API GraphicsPlugin{
		public:
			GraphicsPlugin(const std::filesystem::path& path);

			std::shared_ptr<Renderer> createRenderer();
			std::shared_ptr<Window> createWindow();

		private:
			Module _module;
	};
}

#endif