#include "memory/DynamicArray.hpp"
#include "debug/Logger.hpp"
#include "debug/Instrumentor.hpp"

#include <glm/glm.hpp>

namespace rnd::memory{
	void DynamicArray::init(uint32_t instanceSize, uint32_t instanceCount, uint32_t instancePerChunk){
		PROFILE_FUNCTION();

		this->instanceSize = instanceSize;
		this->instancePerChunk = instancePerChunk;
		this->chunkSize = instanceSize * instancePerChunk;

		int chunkCount = glm::ceil((float)instanceCount / (float)instancePerChunk);

		pushChunks(chunkCount);
	}

	void DynamicArray::shutdown(){
		PROFILE_FUNCTION();
		popFromHere(begin);
	}
	
	void DynamicArray::pushChunk(){
		PROFILE_FUNCTION();

		Chunk* instance = allocChunk();

		instance->next = begin;
		begin = instance;
		chunkCount++;
	}

	void DynamicArray::pushChunks(uint32_t count){
		PROFILE_FUNCTION();

		for (int i=0; i<count; i++){
			pushChunk();
		}
	}

	DynamicArray::Chunk* DynamicArray::allocChunk(){
		PROFILE_FUNCTION();

		Chunk* instance = (Chunk*)malloc(sizeof(Chunk) + chunkSize);

		if (instance == nullptr){
			ERR("malloc error", "failed to allocate dynamic array chunk");
			exit(1);
		}

		return instance;
	}

	void* DynamicArray::getFromIndex(uint32_t index){
		PROFILE_FUNCTION();
		uint32_t chunkID = (float)index / (float)instancePerChunk;
		uint32_t localID = index - (chunkID * instancePerChunk);
		Chunk* chunk = getChunkFromIndex(chunkID);

		return chunk->get(localID, instanceSize);
	}

	DynamicArray::Chunk* DynamicArray::getChunkFromIndex(uint32_t index){
		PROFILE_FUNCTION();

		Chunk* chunk = begin;
		for (int i=0; i<index; i++){
			RND_ASSERT(chunk != nullptr, "dynamic array chunk getter, index overflow");
			chunk = chunk->next;
		}
		return chunk;
	}

	void DynamicArray::resize(uint32_t size){
		PROFILE_FUNCTION();
		int chunkCount = glm::ceil((float)size / (float)instancePerChunk);
		
		if (chunkCount == this->chunkCount){
			return;
		} else if (chunkCount > this->chunkCount){
			int dif = this->chunkCount - chunkCount;
			pushChunks(dif);
		} else {
			Chunk* chunk = getChunkFromIndex(chunkCount);
			popFromHere(chunk);
		}
	}

	void DynamicArray::popFromHere(Chunk* start){
		PROFILE_FUNCTION();
		Chunk* chunk = start;

		while (chunk){
			Chunk* next = chunk->next;
			free(chunk);
			chunk = next;
			chunkCount--;
		}
	}

	void* DynamicArray::Chunk::get(uint32_t index, uint32_t instanceSize){
		char* dataPtr = (char*)this + sizeof(Chunk);
		uint32_t offset = index * instanceSize;
		return dataPtr + offset;
	}
}