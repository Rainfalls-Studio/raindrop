#ifndef __RAINDROP_WRAPPERS_WRAPPERS_HPP__
#define __RAINDROP_WRAPPERS_WRAPPERS_HPP__

#include <Wrappers/common.hpp>
#include <Wrappers/EntityWrapper.hpp>
#include <Wrappers/SceneWrapper.hpp>

namespace Raindrop{
	typedef Wrappers::EntityWrapper Entity;
	typedef Wrappers::SceneWrapper Scene;

	using Wrappers::INVALID_COMPONENT;
	using Wrappers::INVALID_ENTITY;
}

#endif