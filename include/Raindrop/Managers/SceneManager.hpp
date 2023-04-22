#ifndef __RAINDROP_MANAGERS_SCENE_MANAGER_HPP__
#define __RAINDROP_MANAGERS_SCENE_MANAGER_HPP__

#include <common.hpp>
#include <Managers/common.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include <Core/Memory/containers/List.hpp>
#include <Core/Scene/Scene.hpp>
#include <Core/String.hpp>

namespace Raindrop::Managers{
	class RAINDROP_API SceneManager{
		public:
			SceneManager(Core::Memory::Allocator& allocator);
			~SceneManager();

			ScenePtr createScene(usize capacity);
			void destroyScene(ScenePtr scene);

		private:
			Core::Memory::Allocator& _allocator;
			Core::Memory::List<Core::Scene::Scene*> _scenes{_allocator};

			void removeScene(ScenePtr scene);
			
	};
}

#endif