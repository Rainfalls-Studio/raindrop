#ifndef __RAINDROP_MANAGERS_COMMON_HPP__
#define __RAINDROP_MANAGERS_COMMON_HPP__

#include <common.hpp>
#include <Core/Scene/Scene.hpp>
#include <Core/IO/Module.hpp>
#include <Core/IO/Plugin.hpp>

namespace Raindrop::Managers{
	class AssetFactory;
	class ModuleManager;
	class SceneManager;
	class Asset;

	using Raindrop::Core::Scene::Scene;
	using Raindrop::Core::Scene::EntityID;
	using Raindrop::Core::IO::Module;
	using Raindrop::Core::IO::Plugin;

	using AssetPtr = std::weak_ptr<Asset>;
	using ScenePtr = std::weak_ptr<Scene>;
}

#endif