#ifndef __RAINDROP_GRAPHICS_PLUGIN_HPP__
#define __RAINDROP_GRAPHICS_PLUGIN_HPP__

#include <Graphics/common.hpp>

namespace Raindrop::Graphics{
	class RAINDROP_API GraphicsPlugin : public Core::IO::Plugin{
		public:
			GraphicsPlugin() = default;
			virtual ~GraphicsPlugin() = default;

			GraphicsPlugin(const GraphicsPlugin& other) = delete;
			GraphicsPlugin& operator=(const GraphicsPlugin& other) = delete;

			virtual Version version() const override = 0;
			virtual const char* name() const override = 0;

			virtual std::shared_ptr<Renderer> createRenderer() = 0;
	};
}

#endif