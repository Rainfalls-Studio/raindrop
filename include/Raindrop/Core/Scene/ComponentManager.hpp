#ifndef __RAINDROP_CORE_SCENE_COMPONENT_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_COMPONENT_MANAGER_HPP__

#include <common.hpp>
#include <Core/Memory/memory.hpp>

namespace Raindrop::Core::Scene{
	class RAINDROP_API ComponentManager{
		public:
			ComponentManager(usize componentSize, usize componentAlignement, uint32 size = 1024);
			~ComponentManager();

			ComponentManager(const ComponentManager& other) = delete;
			ComponentManager& operator=(const ComponentManager& other) = delete;

			void* get(ID32 id);
			void set(ID32 id, void* component);

			usize capacity() const;
		
		private:
			// * _start;
			std::unique_ptr<void, decltype(std::free)*> _start{nullptr, std::free};
			usize _capacity;
			usize _componentSize;
			usize _componentAlignement;

			void allocate(usize size);
	};
}

#endif