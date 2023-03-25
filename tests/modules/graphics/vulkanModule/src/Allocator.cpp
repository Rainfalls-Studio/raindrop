#include "Allocator.hpp"

#define USE_CUSTOM_ALLOCATOR false
#define STACK_MEMORY 1024 * 16 // 16 Ko

GfxAllocator::GfxAllocator(Allocator& allocator) : _allocator{allocator}, _stack{STACK_MEMORY, allocator.allocate(STACK_MEMORY)}{
	RAINDROP_profile_function();
	_callback.pUserData = this;

	_callback.pfnAllocation = &allocation;
	_callback.pfnReallocation = &reallocation;
	_callback.pfnFree = &free;
	_callback.pfnInternalAllocation = &internalAllocation;
	_callback.pfnInternalFree = &internalFree;
}

GfxAllocator::~GfxAllocator(){
	_allocator.deallocate(_stack.getStart());
}

Allocator& GfxAllocator::get(){
	RAINDROP_profile_function();
	return _allocator;
}

Allocator& GfxAllocator::stack(){
	RAINDROP_profile_function();
	return _stack;
}

VkAllocationCallbacks* GfxAllocator::callback(){
	RAINDROP_profile_function();
	#if USE_CUSTOM_ALLOCATOR
		return &_callback;
	#else
		return nullptr;
	#endif
}

void GfxAllocator::free(void* userData, void* ptr){
	RAINDROP_profile_function();
	GfxAllocator* self = (GfxAllocator*)userData;

}

void* GfxAllocator::allocation(void *userData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope){
	RAINDROP_profile_function();
	GfxAllocator* self = (GfxAllocator*)userData;

}

void* GfxAllocator::reallocation(void *userData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope){
	RAINDROP_profile_function();
	GfxAllocator* self = (GfxAllocator*)userData;

}

void GfxAllocator::internalAllocation(void *userData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope){
	RAINDROP_profile_function();
	GfxAllocator* self = (GfxAllocator*)userData;

}

void GfxAllocator::internalFree(void *userData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope){
	RAINDROP_profile_function();
	GfxAllocator* self = (GfxAllocator*)userData;
}