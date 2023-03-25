#ifndef __VULKANMODULE_ALLOCATOR_HPP__
#define __VULKANMODULE_ALLOCATOR_HPP__

#include "core.hpp"

class GfxAllocator{
	public:
		GfxAllocator(Allocator& allocator);
		~GfxAllocator();

		Allocator& get();
		Allocator& stack();
		VkAllocationCallbacks* callback();

	private:
		Allocator& _allocator;
		StackAllocator _stack;
		VkAllocationCallbacks _callback;

		static void free(void* userData, void* ptr);
		static void* allocation(void *userData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
		static void* reallocation(void *userData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
		static void internalAllocation(void *userData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope);
		static void internalFree(void *userData, size_t size, VkInternalAllocationType allocationType, VkSystemAllocationScope allocationScope);
		
};

#endif