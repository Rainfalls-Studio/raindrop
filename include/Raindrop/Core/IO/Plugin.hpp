#ifndef __RAINDROP_CORE_IO_PLUGIN_HPP__
#define __RAINDROP_CORE_IO_PLUGIN_HPP__

#include <Core/IO/common.hpp>

namespace Raindrop::Core::IO{
	class Plugin{
		public:
			Plugin() = default;
			virtual ~Plugin() = default;

			virtual Version version() const = 0;
			virtual const char* name() const = 0;
	};
}

#endif