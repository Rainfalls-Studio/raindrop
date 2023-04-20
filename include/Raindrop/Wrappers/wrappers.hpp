#ifndef __RAINDROP_WRAPPERS_WRAPPERS_HPP__
#define __RAINDROP_WRAPPERS_WRAPPERS_HPP__

#include <Wrappers/common.hpp>
#include <Wrappers/EntityWrapper.hpp>
#include <Wrappers/SceneWrapper.hpp>
#include <Wrappers/SystemWrapper.hpp>

namespace Raindrop{
	typedef Wrappers::SceneWrapper Scene;
	typedef Wrappers::EntityWrapper Entity;
	typedef Wrappers::SystemWrapper System;
	typedef Wrappers::Signature Signature;

	using Wrappers::INVALID_COMPONENT;
	using Wrappers::INVALID_ENTITY;
}

#endif