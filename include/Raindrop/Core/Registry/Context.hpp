#ifndef __RAINDROP_CORE_REGISTRY_CONTEXT_HPP__
#define __RAINDROP_CORE_REGISTRY_CONTEXT_HPP__

#include <spdlog/spdlog.h>
#include <Raindrop/Core/Registry/common.hpp>

namespace Raindrop::Core::Registry{
	class Context{
		public:
			Context(Core::Context& core);

			Core::Context& core();
			spdlog::logger& logger();

		private:
			Core::Context& _core;
			spdlog::logger _logger;

	};
}

#endif