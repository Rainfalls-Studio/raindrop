#pragma once

#include "../InputStateManager.hpp"

namespace Raindrop::Platform::Interface{
	inline bool InputStateManager::isValid() const noexcept{
		return _impl != nullptr;
	}

	inline KeyState InputStateManager::getKey(Key key) const noexcept{
		assert(isValid());
		return _impl->getKey(key);
	}

	inline MouseButtonState InputStateManager::getButton(const MouseButton button) const noexcept{
		assert(isValid());
		return _impl->getButton(button);
	}

	inline const MousePosition& InputStateManager::getMousePosition() const noexcept{
		assert(isValid());
		return _impl->getMousePosition();
	}

	inline const MousePosition& InputStateManager::getMouseDelta() const noexcept{
		assert(isValid());
		return _impl->getMouseDelta();
	}

	inline float InputStateManager::getMouseScrollDeltaY() const noexcept{
		assert(isValid());
		return _impl->getMouseScrollDeltaY();
	}
}