#ifndef __RAINDROP_GRAPHICS_WINDOW_HPP__
#define __RAINDROP_GRAPHICS_WINDOW_HPP__

#include <common.hpp>
#include <Core/IO/Module.hpp>
#include <Core/Maths/Maths.hpp>

namespace Raindrop::Graphics{
	class RAINDROP_API Window{
		public:
			Window() = default;
			virtual ~Window() = default;

			virtual void setTitle(const char* title) = 0;
			virtual void setSize(Core::Maths::vec2<uint32> size) = 0;
			virtual void setPosition(Core::Maths::vec2<uint32> position) = 0;
			virtual const char* getTitle() const = 0;
			virtual Core::Maths::vec2<uint32> getSize() const = 0;
			virtual Core::Maths::vec2<uint32> getPosition() const = 0;
			virtual const char* getAPI() const = 0;
			virtual bool loaded() const = 0;
	};
}

#endif