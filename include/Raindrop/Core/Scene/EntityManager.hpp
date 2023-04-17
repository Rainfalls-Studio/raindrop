#ifndef __RAINDROP_CORE_SCENE_ENTITY_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_ENTITY_MANAGER_HPP__

#include <common.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include <Core/Memory/containers/List.hpp>

namespace Raindrop::Core::Scene{
	class EntityManager{
		public:
			EntityManager(Memory::Allocator& allocator, usize capacity = 1024);
			~EntityManager();

			/**
			 * @brief create an unused identifies between 0 and the capacity-1
			 * 
			 * @exception std::overflow_error whene the manager does not have free IDs anymore
			 * @return ID32 
			 */
			ID32 create();

			/**
			 * @brief release the ID has an ID that can be used again
			 * @param entity the ID to release
			 * 
			 * @exception std::invalid_argument (ONLY IN DEBUG MODE) whene the user release an ID that is already released
			 */
			void destroy(ID32 entity);

		private:
			Memory::Allocator& _allocator;
			Memory::List<ID32> _freeEntities;

			usize _capacity;
	};
}

#endif