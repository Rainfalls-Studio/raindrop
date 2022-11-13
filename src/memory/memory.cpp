#include "memory/memory.hpp"
#include "debug/Instrumentor.hpp"
#include <string.h>
#include <map>

namespace rnd::memory{
		
	#define COMBINE(x, y) x  ## y
	#define LEVEL1(x,y) COMBINE(x,y)

	#if defined(RND_MEMORY_CHECK) && RND_MEMORY_CHECK == true

		struct MemoryStats{
			std::map<std::string, uint32_t> map;
			int32_t leakedMemory = 0;
		};

		static MemoryStats stats;

		#define ALLOC() stats.leakedMemory++
		#define FREE() stats.leakedMemory--
		#define CALL() stats.map[RND__FUNC__]++
	
		uint32_t getMemStat(const char* fnc){
			return stats.map[fnc];
		}

		uint32_t getLeakedMemoryCount(){
			return stats.leakedMemory;
		}

		bool isMemoryLeaked(){
			return stats.leakedMemory > 0;
		}

	#else
		#define ALLOC()
		#define FREE()
		#define CALL()

		uint32_t getMemStat(const char* fnc){
			return 0;
		}

		uint32_t getLeakedMemoryCount(){
			return 0;
		}

		bool isMemoryLeaked(){
			return false;
		}
	#endif


	void* malloc(uint32_t size){
		PROFILE_FUNCTION();
		CALL();
		ALLOC();
		return ::malloc(size);
	}

	void* calloc(uint32_t count, uint32_t size){
		PROFILE_FUNCTION();
		CALL();
		ALLOC();
		return ::calloc(count, size);
	}

	void* realloc(void* memory, uint32_t newSize){
		PROFILE_FUNCTION();
		CALL();
		return realloc(memory, newSize);
	}

	void free(void* memory){
		PROFILE_FUNCTION();
		CALL();
		FREE();
		::free(memory);
	}

	// ========================================================
	
	void* memcpy(void* dst, const void* src, uint32_t n){
		PROFILE_FUNCTION();
		CALL();
		return ::memcpy(dst, src, n);
	}

	errno_t memcpy_s(void* dst, uint32_t dstS, const void* src, uint32_t size){
		PROFILE_FUNCTION();
		CALL();
		return ::memcpy_s(dst, dstS, src, size);
	}
	
	void* mempcpy(void* dst, const char* src, uint32_t n){
		PROFILE_FUNCTION();
		CALL();
		return ::mempcpy(dst, src, n);
	}
	
	void* memset(void* dst, int32_t val, size_t size){
		PROFILE_FUNCTION();
		CALL();
		return ::memset(dst, val, size);
	}
	
	int memcmp(const void* buf1, const void* buf2, uint32_t size){
		PROFILE_FUNCTION();
		CALL();
		return ::memcmp(buf1, buf2, size);
	}
}