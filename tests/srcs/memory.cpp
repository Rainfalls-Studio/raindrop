#include "test.hpp"

// =======================================================================================
// =======================================================================================
// =======================================================================================

class LinearAllocatorTest : public ::testing::Test {
	protected:
		void SetUp() override {}
		void TearDown() override {
			_allocator.~LinearAllocator();
			free(_mem);
		}

		void* _mem = malloc(MEMORY_KO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_KO, _mem};
};

TEST_F(LinearAllocatorTest, allocation){
	EXPECT_EQ(_allocator.getSize(), MEMORY_KO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);

	struct64* ptr = Raindrop::Core::Memory::allocateNew<struct64>(_allocator);

	EXPECT_NE(ptr, nullptr);

	_allocator.clear();
	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

TEST_F(LinearAllocatorTest, multiple_allocations){
	EXPECT_EQ(_allocator.getSize(), MEMORY_KO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
	
	struct64* ptrs[16];

	for (int i=0; i<16; i++){
		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct64>(_allocator);
		EXPECT_NE(ptrs[i], nullptr);
	}

	_allocator.clear();
	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

TEST_F(LinearAllocatorTest, overflow_allocation){
	EXPECT_EQ(_allocator.getSize(), MEMORY_KO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
	
	struct64* ptrs[17];
	// an overflow of 64 bytes

	for (int i=0; i<17; i++){
		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct64>(_allocator);
	}

	EXPECT_EQ(ptrs[16], nullptr);

	_allocator.clear();
	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

TEST_F(LinearAllocatorTest, full){
	_allocator.allocate(MEMORY_KO, 2);
	_allocator.clear();
}

// =======================================================================================
// =======================================================================================
// =======================================================================================

class PoolAllocatorTest : public ::testing::Test{
	public:
		void SetUp() override {}
		void TearDown() override {
			_allocator.~PoolAllocator();
			free(_mem);
		}

	protected:
		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::PoolAllocator _allocator{sizeof(struct16), alignof(struct16), MEMORY_MO, _mem};
};

TEST_F(PoolAllocatorTest, allocation){
	EXPECT_EQ(_allocator.getSize(), MEMORY_MO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);

	struct16* p = Raindrop::Core::Memory::allocateNew<struct16>(_allocator);

	EXPECT_NE(p, nullptr);
	_allocator.deallocate(p);
	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

TEST_F(PoolAllocatorTest, multiple_allocations){
	EXPECT_EQ(_allocator.getSize(), MEMORY_MO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);

	struct16* ptrs[65536];
	for (int i=0; i<65536; i++){
		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct16>(_allocator);
		EXPECT_NE(ptrs[i], nullptr);
	}

	for (int i=0; i<65536; i++){
		_allocator.deallocate(ptrs[i]);
	}

	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

TEST_F(PoolAllocatorTest, overflow_allocation){
	EXPECT_EQ(_allocator.getSize(), MEMORY_MO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);

	struct16* ptrs[65537];
	// 16 bytes overflow

	for (int i=0; i<65537; i++){
		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct16>(_allocator);
	}

	EXPECT_EQ(ptrs[65536], nullptr);
	
	EXPECT_EQ(_allocator.getUsedMemory(), MEMORY_MO);
	EXPECT_EQ(_allocator.getAllocationCount(), 65536);

	for (int i=0; i<65536; i++){
		_allocator.deallocate(ptrs[i]);
	}

	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

TEST_F(PoolAllocatorTest, unordered_deallocation){
	EXPECT_EQ(_allocator.getSize(), MEMORY_MO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);

	struct16* ptrs[65536];

	for (int i=0; i<65536; i++){
		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct16>(_allocator);
		EXPECT_NE(ptrs[i], nullptr);
	}

	
	EXPECT_EQ(_allocator.getUsedMemory(), MEMORY_MO);
	EXPECT_EQ(_allocator.getAllocationCount(), 65536);

	for (int i=0; i<65536; i+=2){
		_allocator.deallocate(ptrs[i]);
	}

	EXPECT_EQ(_allocator.getUsedMemory(), MEMORY_MO / 2);
	EXPECT_EQ(_allocator.getAllocationCount(), 65536 / 2);

	for (int i=1; i<65536; i+=2){
		_allocator.deallocate(ptrs[i]);
	}

	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

// =======================================================================================
// =======================================================================================
// =======================================================================================

class StackAllocatorTest : public ::testing::Test{
	public:
		void SetUp() override {}
		void TearDown() override {
			_allocator.~StackAllocator();
			free(_mem);
		}

	protected:
		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::StackAllocator _allocator{MEMORY_MO, _mem};
};

TEST_F(StackAllocatorTest, allocation){
	EXPECT_EQ(_allocator.getSize(), MEMORY_MO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);

	struct16* p = Raindrop::Core::Memory::allocateNew<struct16>(_allocator);

	EXPECT_NE(p, nullptr);

	_allocator.deallocate(p);
	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

TEST_F(StackAllocatorTest, multiple_allocation){
	EXPECT_EQ(_allocator.getSize(), MEMORY_MO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);

	struct16* ptrs[32768];
	for (int i=0; i<32768; i++){
		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct16>(_allocator);
		EXPECT_NE(ptrs[i], nullptr);
	}

	for (int i=0; i<32768; i++){
		_allocator.deallocate(ptrs[32767 - i]);
	}

	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

TEST_F(StackAllocatorTest, clear){
	EXPECT_EQ(_allocator.getSize(), MEMORY_MO);
	EXPECT_EQ(_allocator.getStart(), _mem);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);

	struct16* ptrs[32768];
	for (int i=0; i<32768; i++){
		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct16>(_allocator);
		EXPECT_NE(ptrs[i], nullptr);
	}

	_allocator.clear();
	EXPECT_EQ(_allocator.getUsedMemory(), 0);
	EXPECT_EQ(_allocator.getAllocationCount(), 0);
}

// =======================================================================================
// =======================================================================================
// =======================================================================================

TEST(freelistAllocator, init){
	const Raindrop::usize size = MEMORY_MO;
	void* mem = malloc(size);

	{
		Raindrop::Core::Memory::FreelistAllocator allocator(size, mem);
	}
}

// TEST_F(FreelistAllocatorTest, allocate){
// 	int* i = Raindrop::Core::Memory::allocateNew<int>(_allocator);
// 	EXPECT_NE(i, nullptr);

// 	EXPECT_NE(_allocator.getUsedMemory(), 0);
// 	EXPECT_EQ(_allocator.getAllocationCount(), 1);

// 	_allocator.deallocate(i);

// 	printf("%d\n", _allocator.getUsedMemory());
// }

// TEST_F(FreelistAllocatorTest, multiple_allocation){
// 	struct64* ptr[100];

// 	for (int i=0; i<100; i++){
// 		ptr[i] = Raindrop::Core::Memory::allocateNew<struct64>(_allocator);
// 		EXPECT_NE(ptr[i], nullptr);
// 	}

// 	EXPECT_EQ(_allocator.getAllocationCount(), 100);
// 	EXPECT_NE(_allocator.getUsedMemory(), 0);

// 	for (int i=0; i<100; i++){
// 		_allocator.deallocate(ptr[i]);
// 	}
// }

// TEST_F(FreelistAllocatorTest, unordered_multiple_allocation){
// 	struct64* ptr[10];

// 	for (int i=0; i<10; i++){
// 		ptr[i] = Raindrop::Core::Memory::allocateNew<struct64>(_allocator);
// 		EXPECT_NE(ptr[i], nullptr);
// 	}

// 	EXPECT_EQ(_allocator.getAllocationCount(), 10);
// 	EXPECT_NE(_allocator.getUsedMemory(), 0);

// 	for (int i=5; i<10; i++){
// 		_allocator.deallocate(ptr[i]);
// 	}

// 	for (int i=0; i<5; i++){
// 		_allocator.deallocate(ptr[i]);
// 	}
// }

// TEST_F(FreelistAllocatorTest, multiple_types_allocation){
// 	void* ptrs[10];

// 	for (int i=0; i<4; i++){
// 		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct64>(_allocator);
// 		EXPECT_NE(ptrs[i], nullptr);
// 	}

// 	for (int i=4; i<7; i++){
// 		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct8>(_allocator);
// 		EXPECT_NE(ptrs[i], nullptr);
// 	}

// 	for (int i=7; i<10; i++){
// 		ptrs[i] = Raindrop::Core::Memory::allocateNew<struct512>(_allocator);
// 		EXPECT_NE(ptrs[i], nullptr);
// 	}

// 	EXPECT_EQ(_allocator.getAllocationCount(), 10);
// 	EXPECT_NE(_allocator.getUsedMemory(), 0);

// 	for (int i=0; i<10; i++){
// 		_allocator.deallocate(ptrs[i]);
// 	}
// }

// TEST_F(FreelistAllocatorTest, random_test){
// 	void** ptrs = Raindrop::Core::Memory::allocateArray<void*>(_allocator, 1000);

// 	for (Raindrop::uint32 i=0; i<1000; i++){
// 		ptrs[i] = _allocator.allocate((rand() % 500) + 1);
// 	}

// 	for (Raindrop::uint32 i=0; i<1000; i+=(rand() % 5) + 1){
// 		_allocator.deallocate(ptrs[i]);
// 		ptrs[i] = nullptr;
// 	}

// 	for (Raindrop::uint32 i=0; i<1000; i+=(rand() % 3) + 1){
// 		if (ptrs[i] == nullptr){
// 			ptrs[i] = _allocator.allocate((rand() % 500) + 1);
// 			continue;
// 		}
// 		_allocator.deallocate(ptrs[i]);
// 		ptrs[i] = nullptr;
// 	}

// 	for (Raindrop::uint32 i=0; i<1000; i++){
// 		if (ptrs[i]){
// 			_allocator.deallocate(ptrs[i]);
// 		}
// 	}

// 	_allocator.deallocate(ptrs);
// }

// =======================================================================================
// =======================================================================================
// =======================================================================================

// once the test goes here, the allocators are supposed to be secure

class ArrayTest : public ::testing::Test{
	public:
		void SetUp() override {}
		void TearDown() override {
			_arr.clear();
			_arr.~Array();
			_allocator.clear();
			_allocator.~LinearAllocator();
			free(_mem);
		}

	protected:
		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_MO, _mem};
		Raindrop::Core::Memory::Array<struct16> _arr{_allocator};
};

TEST_F(ArrayTest, push){
	EXPECT_EQ(_arr.size(), 0);
	EXPECT_TRUE(_arr.empty());

	struct16 c;
	c.a[0] = 1;
	_arr.push(c);

	EXPECT_EQ(_arr.size(), 1);
	EXPECT_FALSE(_arr.empty());

	EXPECT_EQ(_arr[0].a[0], 1);
}

TEST_F(ArrayTest, multiple_push){
	EXPECT_EQ(_arr.size(), 0);
	EXPECT_TRUE(_arr.empty());

	for (int i=0; i<100; i++){
		struct16 c;
		c.a[0] = i;
		_arr.push(c);
	}

	EXPECT_EQ(_arr.size(), 100);
	EXPECT_FALSE(_arr.empty());

	EXPECT_EQ(_arr[0].a[0], 0);
	EXPECT_EQ(_arr[5].a[0], 5);
	EXPECT_EQ(_arr[90].a[0], 90);
}

TEST_F(ArrayTest, pop){
	EXPECT_EQ(_arr.size(), 0);
	EXPECT_TRUE(_arr.empty());

	struct16 c;
	c.a[0] = 1;
	_arr.push(c);

	EXPECT_EQ(_arr.size(), 1);
	EXPECT_FALSE(_arr.empty());

	_arr.pop();
}

TEST_F(ArrayTest, multiple_pop){
	EXPECT_EQ(_arr.size(), 0);
	EXPECT_TRUE(_arr.empty());

	for (int i=0; i<100; i++){
		struct16 c;
		c.a[0] = i;
		_arr.push(c);
	}

	EXPECT_EQ(_arr.size(), 100);
	EXPECT_FALSE(_arr.empty());

	for (int i=0; i<100; i++){
		_arr.pop();
	}
}

TEST_F(ArrayTest, reserve){
	EXPECT_EQ(_arr.size(), 0);
	EXPECT_TRUE(_arr.empty());

	_arr.reserve(100);

	EXPECT_EQ(_arr.capacity(), 100);
	EXPECT_EQ(_arr.size(), 0);

	for (int i=0; i<100; i++){
		_arr.push(struct16{});
	}

	EXPECT_EQ(_arr.capacity(), 100);
	EXPECT_EQ(_arr.size(), 100);
}

TEST_F(ArrayTest, resize){
	EXPECT_EQ(_arr.size(), 0);
	EXPECT_TRUE(_arr.empty());

	_arr.resize(100);

	EXPECT_EQ(_arr.size(), 100);
	EXPECT_GE(_arr.capacity(), 100);

	for (int i=0; i<100; i++){
		_arr[i] = {};
	}

	EXPECT_EQ(_arr.capacity(), 100);
	EXPECT_EQ(_arr.size(), 100);
}

// =======================================================================================
// =======================================================================================
// =======================================================================================

class HashMapTest : public ::testing::Test{
	public:
		void SetUp() override {}
		void TearDown() override {
			_map.~HashMap();
			_allocator.clear();
			_allocator.~LinearAllocator();
			free(_mem);
		}

	protected:
		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_MO, _mem};
		Raindrop::Core::Memory::HashMap<int, struct8> _map{_allocator};
};

TEST_F(HashMapTest, insert){
	EXPECT_EQ(_map.size(), 0);
	_map.insert(5, {});
	EXPECT_EQ(_map.size(), 1);
}

TEST_F(HashMapTest, remove){
	EXPECT_EQ(_map.size(), 0);
	_map.insert(5, {});

	EXPECT_EQ(_map.size(), 1);
	_map.remove(5);

	EXPECT_EQ(_map.size(), 0);
}

TEST_F(HashMapTest, multiple_insert){
	EXPECT_EQ(_map.size(), 0);

	for (int i=0; i<100; i++){
		_map.insert(i, {});
	}
	
	EXPECT_EQ(_map.size(), 100);
}

TEST_F(HashMapTest, multiple_remove){
	EXPECT_EQ(_map.size(), 0);

	for (int i=0; i<100; i++){
		_map.insert(i, {});
	}

	EXPECT_EQ(_map.size(), 100);

	for (int i=0; i<50; i++){
		_map.remove(i*2);
	}
	
	EXPECT_EQ(_map.size(), 50);
	
	for (int i=0; i<100; i++){
		_map.insert(i, {});
	}

	EXPECT_EQ(_map.size(), 150);
}

TEST_F(HashMapTest, clear){
	EXPECT_EQ(_map.size(), 0);

	for (int i=0; i<100; i++){
		_map.insert(i, {});
	}

	EXPECT_EQ(_map.size(), 100);
	_map.clear();
	EXPECT_EQ(_map.size(), 0);
	
	for (int i=0; i<50; i++){
		_map.insert(i, {});
	}
}

TEST_F(HashMapTest, reserve){
	EXPECT_EQ(_map.size(), 0);

	_map.reserve(100);

	for (int i=0; i<100; i++){
		_map.insert(i, {});
	}

	EXPECT_EQ(_map.size(), 100);
	_map.clear();
	EXPECT_EQ(_map.size(), 0);
}

TEST_F(HashMapTest, has){
	EXPECT_EQ(_map.size(), 0);

	for (int i=0; i<100; i++){
		_map.insert(i, {});
	}

	for (int i=0; i<100; i++){
		EXPECT_TRUE(_map.has(i));
	}

	EXPECT_FALSE(_map.has(150));
}

TEST_F(HashMapTest, get){
	EXPECT_EQ(_map.size(), 0);

	for (int i=0; i<100; i++){
		struct8 c;
		c.a[0] = (char)i;
		_map.insert(i, c);
	}

	for (int i=0; i<100; i++){
		struct8* c = _map.get(i);
		EXPECT_EQ(c->a[0], i);
	}
}

// =======================================================================================
// =======================================================================================
// =======================================================================================

class ListTest : public ::testing::Test{
	public:
		void SetUp() override {}
		void TearDown() override {
			_list.~List();
			_allocator.clear();
			_allocator.~LinearAllocator();
			free(_mem);
		}

	protected:
		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_MO, _mem};
		Raindrop::Core::Memory::List<struct64> _list{_allocator};
};

TEST_F(ListTest, push){
	EXPECT_EQ(_list.size(), 0);
	EXPECT_TRUE(_list.empty());

	_list.push({});

	EXPECT_EQ(_list.size(), 1);
}

TEST_F(ListTest, pop){
	EXPECT_EQ(_list.size(), 0);
	EXPECT_TRUE(_list.empty());

	_list.push({});
	EXPECT_EQ(_list.size(), 1);

	_list.pop();
	EXPECT_EQ(_list.size(), 0);
}

TEST_F(ListTest, multiple_push){
	EXPECT_EQ(_list.size(), 0);
	EXPECT_TRUE(_list.empty());

	for (int i=0; i<100; i++){
		_list.push({});
	}
	EXPECT_EQ(_list.size(), 100);
}

TEST_F(ListTest, multiple_pop){
	EXPECT_EQ(_list.size(), 0);
	EXPECT_TRUE(_list.empty());

	for (int i=0; i<100; i++){
		_list.push({});
	}

	EXPECT_EQ(_list.size(), 100);

	for (int i=0; i<50; i++){
		_list.pop();
	}

	EXPECT_EQ(_list.size(), 50);
}

TEST_F(ListTest, clear){
	EXPECT_EQ(_list.size(), 0);
	EXPECT_TRUE(_list.empty());

	for (int i=0; i<100; i++){
		_list.push({});
	}

	EXPECT_EQ(_list.size(), 100);
	_list.clear();

	EXPECT_TRUE(_list.empty());
}

// =======================================================================================
// =======================================================================================
// =======================================================================================

class PoolTest : public ::testing::Test{
	public:
		void SetUp() override {}
		void TearDown() override {
			_allocator.clear();
		}

	protected:
		void* _mem = malloc(MEMORY_MO);
		Raindrop::Core::Memory::LinearAllocator _allocator{MEMORY_MO, _mem};
		Raindrop::Core::Memory::Pool<struct64> _pool{500, _allocator};
};

TEST_F(PoolTest, init){
	EXPECT_FALSE(_pool.empty());
	EXPECT_EQ(_pool.size(), 500);
}

TEST_F(PoolTest, get){
	struct64* ptrs[500];

	for (int i=0; i<500; i++){
		ptrs[i] = _pool.get();
		EXPECT_NE(ptrs[i], nullptr);
	}

	EXPECT_TRUE(_pool.empty());

	for (int i=0; i<500; i++){
		_pool.push(ptrs[i]);
	}

	EXPECT_EQ(_pool.size(), 500);
	EXPECT_FALSE(_pool.empty());
}

// =======================================================================================
// =======================================================================================
// =======================================================================================

TEST(bitset, init){
	Raindrop::Core::Memory::Bitset<3000> bitset;
}

TEST(bitset, set_and_get){
	Raindrop::Core::Memory::Bitset<3000> bitset;

	for (int i=0; i<3000; i++){
		EXPECT_EQ(bitset.get(i), 0);
	}

	for (int i=0; i<3000; i+=5){
		bitset.set(i);
	}

	for (int i=0; i<3000; i+=5){
		EXPECT_EQ(bitset.get(i), 1);
	}
}