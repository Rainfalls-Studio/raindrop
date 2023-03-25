#ifndef __RAINDROP_GRAPHCICS_RENDERER_HPP__
#define __RAINDROP_GRAPHCICS_RENDERER_HPP__

#include "Core/core.hpp"
#include "Core/common.hpp"
#include "Core/IO/Module.hpp"
#include "Core/Memory/allocators/Allocator.hpp"
#include "Core/Maths/Maths.hpp"

namespace Raindrop::Graphics{
	class Renderer;
	class Window;
	class Plugin;

	class Context{
		public:
			Context();
			virtual ~Context() = default;

			virtual bool begin() = 0;
			virtual void end() = 0;
			virtual void setClearColor(Core::Maths::vec4<float32> color) = 0;
			virtual void enableVSync(bool enable) = 0;
			virtual bool isVSyncEnabled() const = 0;

			Plugin* loadPlugin(Core::Memory::Allocator& allocator, Core::IO::Module& module, const char* name);

			static Context* create(Core::Memory::Allocator& allocator, Renderer& renderer, Window& window);
	};
}

#endif