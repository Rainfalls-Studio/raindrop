#include "Core/Scene/ComponentManager.hpp"
#include <cstring>

namespace Raindrop::Core::Scene{
	RAINDROP_API ComponentManager::ComponentManager(Memory::Allocator& allocator, usize componentSize, usize componentAlignement, uint32 size) : _allocator{allocator}, _componentSize{componentSize}, _componentAlignement{componentAlignement}{
		RAINDROP_profile_function();
		_start = nullptr;
		_capacity = 0;
		allocate(size);
	}

	RAINDROP_API ComponentManager::~ComponentManager(){
		RAINDROP_profile_function();
		if (_start) _allocator.deallocate(_start);
		_start = nullptr;
	}

	RAINDROP_API void ComponentManager::allocate(usize size){
		RAINDROP_profile_function();

		void* newStart = _allocator.allocate(size * _componentSize, _componentAlignement);
		if (!newStart){
			RAINDROP_log(ERROR, MEMORY, "failed to allocate the buffer of a scene component manager");
			return;
		}

		if (_start){
			memcpy(newStart, _start, _capacity);
			_allocator.deallocate(_start);
		}

		_capacity = size;
		_start = newStart;
	}

	RAINDROP_API void* ComponentManager::get(ID32 id){
		RAINDROP_profile_function();
		if (id >= _capacity) throw std::out_of_range("the given component id is out of the bounds of the component manager");
		return static_cast<void*>(static_cast<char*>(_start) + _componentSize * id);
	}

	RAINDROP_API void ComponentManager::set(ID32 id, void* component){
		RAINDROP_profile_function();
		if (id >= _capacity) throw std::out_of_range("the given component id is out of the bounds of the component manager");
		if (!component) throw std::invalid_argument("the given component pointer in null, cannot copy");
		void* ptr = get(id);
		memcpy(ptr, component, _componentSize);
	}

	RAINDROP_API usize ComponentManager::capacity() const{
		RAINDROP_profile_function();
		return _capacity;
	}
}