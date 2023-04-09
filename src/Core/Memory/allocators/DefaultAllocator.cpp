#include "Core/Memory/allocators/DefaultAllocator.hpp"
#include "Core/Memory/memory.hpp"
#include "Core/Maths/Maths.hpp"

namespace Raindrop::Core::Memory{
	DefaultAllocator::DefaultAllocator() : Allocator{(usize)-1, nullptr}{}
	DefaultAllocator::~DefaultAllocator(){}

	void* DefaultAllocator::allocate(usize size, uint8 alignement){
		if (size == 0) throw std::invalid_argument("cannot allocate an element with a null size");
		if (alignement == 0) throw std::invalid_argument("cannot allocate memory with a null alignement");
		if (!Maths::isPowerOfTwo(alignement)) throw std::invalid_argument("the alignement has to be a power of two");

		usize totalSize = alignSize(size + sizeof(uint8), alignement);
		void* mem = Memory::allocate(totalSize);

		if (!mem) throw std::bad_alloc();
		*(uint8*)mem = alignAdjustment(mem, alignement);
		
		allocationCount++;
		return ((uint8*)mem)+1;
	}

	void DefaultAllocator::deallocate(void* p){
		if (p == nullptr) throw std::invalid_argument("cannot deallocate a null pointer");
		uint8 adjustment = *(((uint8*)p)-1);
		p = (void*)((uint8*)p - adjustment - sizeof(uint8));
		allocationCount--;
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