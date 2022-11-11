#include "memory/Pool.hpp"
#include "debug/Logger.hpp"
#include "debug/Instrumentor.hpp"
#include <stdlib.h>

namespace rnd::memory{
	void Pool::init(uint32_t instanceSize, uint32_t instanceCount){
		PROFILE_FUNCTION();
		LOG(debug::Info, "initialize memory pool", "none");

		destroy();

		this->instanceSize = instanceSize;

		pushChunks(instanceCount);
	}

	void Pool::shutdown(){
		PROFILE_FUNCTION();
		LOG(debug::Info, "shutdown memory pool", "none");
		destroy();
	}

	void Pool::destroy(){
		PROFILE_FUNCTION();
	}

	void Pool::pushChunks(uint32_t count){
		PROFILE_FUNCTION();
		for (int i=0; i<count; i++){
			pushOneChunk();
		}
	}

	void Pool::pushOneChunk(){
		PROFILE_FUNCTION();
		if (begin){
			Chunk* next = begin->next;
			begin->next = allocOneChunk();
			begin->next = next;
		} else {
			begin = allocOneChunk();
		}

		instanceCount++;
	}

	Pool::Chunk* Pool::allocOneChunk(){
		PROFILE_FUNCTION();
		Chunk* instance = (Chunk*)malloc(getChunkSize());

		if (instance == nullptr){
			LOG(debug::Critical, "malloc error", "failed to allocate pool chunk");
			exit(EXIT_FAILURE);
		}

		instance->next = nullptr;
		return instance;
	}

	uint32_t Pool::getChunkSize(){
		PROFILE_FUNCTION();
		return sizeof(Chunk) + instanceSize;
	}

	Pool::Chunk* Pool::popOneChunk(){
		PROFILE_FUNCTION();
		if (!begin) return nullptr;

		Chunk* temp = begin;
		begin = begin->next;
		instanceCount--;
		return temp;
	}
	
	bool Pool::destroyOneChunk(){
		PROFILE_FUNCTION();
		if (!begin) return false;

		Chunk* temp = begin;
		begin = begin->next;
		free(temp);
		return true;
	}

	void Pool::destroyChunks(uint32_t count){
		PROFILE_FUNCTION();
		for (int i=0; i<count; i++){
			if (!destroyOneChunk()) break;
		}
	}
}
