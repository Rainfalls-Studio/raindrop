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

			template<typename... Args>
			static std::shared_ptr<Plugin> createPlugin(Module& module, const char* name, Args... args){
				using LoadFnc = std::shared_ptr<Plugin>(*)(Args...);
				LoadFnc fnc = reinterpret_cast<LoadFnc>(module.getFnc(name));
				return fnc(args...);
			}
	};
}

#endif