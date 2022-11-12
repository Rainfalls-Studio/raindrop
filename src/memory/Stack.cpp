#include "memory/Stack.hpp"
#include "debug/Instrumentor.hpp"

namespace rnd::memory{
	void Stack::init(uint32_t size){
		PROFILE_FUNCTION();
		memory = (char*)malloc(size);
		ptr = memory;
		maxSize = size;
		usedSize = 0;
	}

	void Stack::shutdown(){
		PROFILE_FUNCTION();
		free(memory);
		memory = nullptr;
		ptr = nullptr;
		usedSize = 0;
		maxSize = 0;
	}

	void Stack::reset(uint32_t to){
		PROFILE_FUNCTION();
		ptr = memory + to;
	}

	uint32_t Stack::size(){
		PROFILE_FUNCTION();
		return usedSize;
	}
	
	void* Stack::allocate(uint32_t size){
		PROFILE_FUNCTION();
		void* data = ptr;
		ptr += size;
		return data;
	}
}