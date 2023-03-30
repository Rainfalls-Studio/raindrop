#ifndef __RAINDROP_CORE_SCENE_COMPONENT_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENT_MANAGER_HPP__

#include "../core.hpp"
#include "../common.hpp"
#include "../Memory/allocators/Allocator.hpp"
#include "../Memory/containers/Array.hpp"
#include "../Memory/containers/HashMap.hpp"

namespace Raindrop::Core::Scene{
	class ComponentManager{
		public:
			ComponentManager(Memory::Allocator& allocator, usize componentSize, usize componentAlignement, uint32 size = 4096);
			~ComponentManager();

			void* get(ID32 id);
			void set(ID32 id, void* component);

			usize capacity() const;
		
		private:
			Memory::Allocator& _allocator;
			void* _start;
			usize _capacity;
			usize _componentSize;
			usize _componentAlignement;

			void allocate(usize size);
	};
}

#endif