#include <Raindrop/Graphics/Context.hpp>
#include <Raindrop/Core/Context.hpp>

namespace Raindrop::Graphics{
	Context::Context(Core::Context& core) :
		_core{core},
		_logger("Raindrop::Graphics"),
		_registry{_core.registry()["Graphics"]},
		_internal(*this),
		_utils(*this)
	{}

	Core::Context& Context::core(){
		return _core;
	}

	spdlog::logger& Context::logger(){
		return _logger;
	}

	Core::Registry::Node& Context::registry(){
		return _registry;
	}

	VkAllocationCallbacks*& Context::allocationCallbacks(){
		return _allocationCallbacks;
	}

	Internal::Context& Context::internal(){
		return _internal;
	}

	Utils::Context& Context::utils(){
		return _utils;
	}
}