#include "Core/Scene/ComponentManager.hpp"
#include <Core/Debug/profiler.hpp>
#include <cstring>

namespace Raindrop::Core::Scene{
	RAINDROP_API ComponentManager::ComponentManager(usize componentSize, usize componentAlignement, uint32 size) : _componentSize{componentSize}, _componentAlignement{componentAlignement}{
		RAINDROP_profile_function();
		_start = nullptr;
		_capacity = 0;
		allocate(size);
	}

	RAINDROP_API ComponentManager::~ComponentManager(){
		RAINDROP_profile_function();
		_start.release();
	}

	RAINDROP_API void ComponentManager::allocate(usize size){
		RAINDROP_profile_function();

		std::unique_ptr<void, decltype(std::free)*> newStart(_aligned_malloc(size * _componentSize, _componentAlignement), std::free);
		std::memcpy(newStart.get(), _start.get(), std::min(_capacity * _componentSize, size * _componentSize));
		_start = std::move(newStart);

		_capacity = size;
	}

	RAINDROP_API void* ComponentManager::get(ID32 id){
		RAINDROP_profile_function();
		if (id >= _capacity) throw std::out_of_range("the given component id is out of the bounds of the component manager");
		return static_cast<void*>(static_cast<char*>(_start.get()) + _componentSize * id);
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