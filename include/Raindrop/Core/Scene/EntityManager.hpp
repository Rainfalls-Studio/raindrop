#ifndef __RAINDROP_CORE_SCENE_ENTITY_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_ENTITY_MANAGER_HPP__

#include <Core/Scene/common.hpp>
#include <Core/Memory/memory.hpp>

namespace Raindrop::Core::Scene{
	class RAINDROP_API EntityManager{
		public:
			EntityManager(usize capacity = 1024);
			~EntityManager();

			/**
			 * @brief create an unused identifies between 0 and the capacity-1
			 * 
			 * @exception std::overflow_error when the manager does not have free IDs anymore
			 * @return EntityID
			 */
			EntityID create();

			/**
			 * @brief release the ID has an ID that can be used again
			 * @param entity the ID to release
			 * 
			 * @exception std::invalid_argument (ONLY IN DEBUG MODE) whene the user release an ID that is already released
			 */
			void destroy(EntityID entity);

		private:
			std::queue<EntityID> _freeEntities;
			usize _capacity;
	};
}

#endif