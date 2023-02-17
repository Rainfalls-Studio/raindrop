#include "DefaultAllocator.hpp"
#include "../memory.hpp"

namespace Raindrop::Core::Memory{
	DefaultAllocator::DefaultAllocator() : Allocator{0, nullptr}{}
	DefaultAllocator::~DefaultAllocator(){}

	void* DefaultAllocator::allocate(usize size, uint8 alignement){
		return Memory::allocate(size);
	}

	void DefaultAllocator::deallocate(void* p){
		Memory::deallocate(p);
	}

	DefaultAllocator& DefaultAllocator::get(){
		static DefaultAllocator instance;
		return instance;
	}

	Allocator& defaultAllocator(){
		return DefaultAllocator::get();
	}
}