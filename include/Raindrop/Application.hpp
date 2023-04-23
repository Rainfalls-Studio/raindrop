#ifndef __RAINDROP_APPLICATION_HPP__
#define __RAINDROP_APPLICATION_HPP__

#include <common.hpp>
#include <Core/String.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include <Managers/SceneManager.hpp>
#include <Managers/AssetManager.hpp>
#include <Wrappers/wrappers.hpp>

namespace Raindrop{
	class RAINDROP_API Application{
		public:
			Application(const char* name, Version version, Core::Memory::Allocator& allocator = RAINDROP_default_allocator);
			~Application();

			const char* name() const;
			Version version() const;

			Scene createScene(usize capacity);
			void destroySceen(Scene scene);
		
		private:
			Core::Memory::Allocator& _allocator;
			Core::String _name{_allocator};
			Version _version;

			Managers::SceneManager* _sceneManager;
			Managers::AssetManager* _assetManager;
	};
}

#endif