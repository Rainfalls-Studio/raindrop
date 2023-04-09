#include <typeinfo>
#include <stdexcept>

#include <gtest/gtest.h>
#include <Raindrop.hpp>

using namespace Raindrop::Core::Memory;

TEST(defaultAllocator, init){
	EXPECT_NO_THROW(DefaultAllocator allocator);
}

TEST(defaultAllocator, validAllocation){
	DefaultAllocator allocator;
	void* ptr = allocator.allocate(600);
	EXPECT_NE(ptr, nullptr);
	allocator.deallocate(ptr);
}

TEST(defaultAllocator, invalidAlignementAllocation){
	DefaultAllocator allocator;
	EXPECT_THROW(allocator.allocate(600, 0), std::invalid_argument);
}

TEST(defaultAllocator, invalidAlignementAllocationNotPowerOfTwo){
	DefaultAllocator allocator;
	EXPECT_THROW(allocator.allocate(600, 7), std::invalid_argument);
}

TEST(freeListAllocator, init){
	void* mem = allocate(1024);
	{
		EXPECT_NO_THROW(FreelistAllocator allocator(1024, mem));
	}
	deallocate(mem);
}

TEST(freeListAllocator, validAllocation){
	void* mem = allocate(1024);
	{
		FreelistAllocator allocator(1024, mem);
		void* ptr = nullptr;
		EXPECT_NO_THROW(ptr = allocator.allocate(sizeof(Raindrop::uint64), alignof(Raindrop::uint64)));
		EXPECT_NE(ptr, nullptr);
		EXPECT_NO_THROW(allocator.deallocate(ptr));
		
	}
	deallocate(mem);
}

TEST(freeListAllocator, invalidAllocationBadSize){
	void* mem = allocate(1024);
	{
		FreelistAllocator allocator(1024, mem);
		void* ptr = nullptr;
		EXPECT_THROW(ptr = allocator.allocate(0, alignof(Raindrop::uint64)), std::invalid_argument);
		EXPECT_EQ(ptr, nullptr);
		EXPECT_THROW(allocator.deallocate(ptr), std::invalid_argument);
		
	}
	deallocate(mem);
}

TEST(freeListAllocator, invalidAllocationTooLargeSize){
	void* mem = allocate(1024);
	{
		FreelistAllocator allocator(1024, mem);
		void* ptr = nullptr;
		EXPECT_THROW(ptr = allocator.allocate(4048, alignof(Raindrop::uint64)), std::bad_alloc);
		EXPECT_EQ(ptr, nullptr);
		EXPECT_THROW(allocator.deallocate(ptr), std::invalid_argument);
		
	}
	deallocate(mem);
}