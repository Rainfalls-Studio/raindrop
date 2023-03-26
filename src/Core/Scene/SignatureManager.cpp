#include "SignatureManager.hpp"
#include "../Maths/Maths.hpp"
#include <cstring>

namespace Raindrop::Core::Scene{
	SignatureManager::SignatureManager(Memory::Allocator& allocator, usize capacity, usize componentCount) : _allocator{allocator}{
		RAINDROP_profile_function();
		_start = nullptr;
		_capacity = 0;
		_componentCount = 0;

		resize(capacity, componentCount);
	}

	SignatureManager::~SignatureManager(){
		RAINDROP_profile_function();
		if (_start) _allocator.deallocate(_start);
		_start = nullptr;
	}

	void SignatureManager::resize(usize size, usize componentCount){
		RAINDROP_profile_function();

		usize componentBytes = glm::ceil(static_cast<float>(componentCount) / 8.f);
		usize _componentBytes = glm::ceil(static_cast<float>(_componentCount) / 8.f);

		void* newPtr = _allocator.allocate(size * componentBytes, alignof(uint8));
		if (!newPtr) return;

		if (_start){
			for (usize i=0; i<_componentCount; i++){
				memcpy(static_cast<uint8*>(newPtr) + size * componentBytes * i, static_cast<uint8*>(_start) + _capacity * _componentBytes * i, Maths::min(size * componentBytes, _capacity * _componentBytes));
			}
			_allocator.deallocate(_start);
		}

		_start = newPtr;
		_capacity = size;
		_componentCount = componentCount;
	}

	void SignatureManager::setComponentState(ID32 entity, usize componentID, bool state){
		RAINDROP_profile_function();

		usize componentBytes = glm::ceil(static_cast<float>(_componentCount) / 8.f);
		uint8* component = static_cast<uint8*>(_start) + componentBytes * _capacity * componentID;

		uint8& byte = component[entity / 8];
		byte ^= (-state ^ byte) & (1UL << (entity % 8));
	}

	bool SignatureManager::getComponentState(ID32 entity, usize componentID) const{
		RAINDROP_profile_function();

		usize componentBytes = glm::ceil(static_cast<float>(_componentCount) / 8.f);
		uint8* component = static_cast<uint8*>(_start) + componentBytes * _capacity * componentID;

		uint8& byte = component[entity / 8];
		return (byte >> entity % 8) & 1U;
	}
}