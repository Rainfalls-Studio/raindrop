#ifndef __RAINDROP_CORE_SCENE_ENTITY_MANAGER8HPP__
#define __RAINDROP_CORE_SCENE_ENTITY_MANAGER8HPP__

#include "../core.hpp"
#include "../common.hpp"
#include "../Memory/allocators/Allocator.hpp"
#include "../Memory/containers/List.hpp"

namespace Raindrop::Core::Scene{
	class EntityManager{
		public:
			EntityManager(Memory::Allocator& allocator, usize capacity = 1024);
			~EntityManager();

			ID32 create();
			void destroy(ID32 entity);

		private:
			Memory::Allocator& _allocator;
			Memory::List<ID32> _freeEntities;

			usize _capacity;
	};
}

#endif