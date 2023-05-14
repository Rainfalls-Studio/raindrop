#ifndef __RAINDROP_WRAPPERS_WRAPPERS_HPP__
#define __RAINDROP_WRAPPERS_WRAPPERS_HPP__

#include <Wrappers/common.hpp>
#include <Wrappers/EntityWrapper.hpp>
#include <Wrappers/SceneWrapper.hpp>
#include <Wrappers/SystemWrapper.hpp>
#include <Wrappers/ModuleWrapper.hpp>

namespace Raindrop{
	using Scene = Wrappers::SceneWrapper;
	using Entity = Wrappers::EntityWrapper;
	using Signature = Wrappers::Signature;
	using SystemBase = Wrappers::SystemBase;
	using Module = Wrappers::ModuleWrapper;

	using Wrappers::INVALID_COMPONENT;
	using Wrappers::INVALID_ENTITY;
}

#endif