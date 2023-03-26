#ifndef __RAINDROP_CORE_SCENE_SIGNATURE_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_SIGNATURE_MANAGER_HPP__

#include "../core.hpp"
#include "../common.hpp"
#include "../Memory/allocators/Allocator.hpp"

namespace Raindrop::Core::Scene{
	class SignatureManager{
		public:
			SignatureManager(Memory::Allocator& allocator, usize capacity, usize componentCount);
			~SignatureManager();

			void setComponentState(ID32 entity, usize componentID, bool state);
			bool getComponentState(ID32 entity, usize componentID) const;
		
		private:
			Memory::Allocator& _allocator;
			void* _start;
			usize _capacity;
			usize _componentCount;

			void resize(usize size, usize componentCount);
	};
}

#endif