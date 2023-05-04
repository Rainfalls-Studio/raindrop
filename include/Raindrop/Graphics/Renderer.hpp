#ifndef __RAINDROP_GRAPHICS_RENDERER_HPP__
#define __RAINDROP_GRAPHICS_RENDERER_HPP__

#include <Graphics/common.hpp>

namespace Raindrop::Graphics{
	class RAINDROP_API Renderer{
		public:
			Renderer() = default;
			virtual ~Renderer() = default;

			Renderer(const Renderer& other) = delete;
			Renderer& operator=(const Renderer& other) = delete;

			virtual const char* name() const = 0;
			virtual std::shared_ptr<Context> createContext(std::shared_ptr<Window> window) = 0;

	};
}

#endif