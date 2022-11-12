#include "memory/DynamicArray.hpp"
#include "memory/ChunkAllocator.hpp"
#include "debug/Logger.hpp"
#include "debug/Instrumentor.hpp"
#include <math.h>

namespace rnd::memory{
	void DynamicArray::init(Allocator* allocator, uint32_t elementSize, uint32_t size, uint32_t elementPerChunk){
		PROFILE_FUNCTION();
		customAllocator = true;
		this->allocator = allocator;
		this->elementSize = elementSize;
		this->elementPerChunk = elementPerChunk;

		int chunkCount = std::ceil((float)size / (float)elementPerChunk);
		pushChunks(chunkCount);
	}

	void DynamicArray::init(uint32_t elementSize, uint32_t size, uint32_t elementPerChunk){
		PROFILE_FUNCTION();
		createAllocator(size);
		this->elementSize = elementSize;
		this->elementPerChunk = elementPerChunk;
		
		int chunkCount = std::ceil((float)size / (float)elementPerChunk);
		pushChunks(chunkCount);
	}

	void DynamicArray::shutdown(){
		PROFILE_FUNCTION();
		if (!customAllocator){
			allocator->shutdown();
			free(allocator);
		}
		allocator = nullptr;
	}

	void DynamicArray::createAllocator(uint32_t size){
		PROFILE_FUNCTION();
		allocator = (ChunkAllocator*)malloc(sizeof(ChunkAllocator));
		
		((ChunkAllocator*)allocator)->init(elementSize * elementPerChunk + sizeof(Chunk), size / 4, 4);
	}

	void DynamicArray::pushBack(Chunk* chunk){
		PROFILE_FUNCTION();

		if (begin){
			end->next = chunk;
			end = chunk;
		} else {
			begin = chunk;
			end = chunk;
		}
		chunkCount++;
	}

	DynamicArray::Chunk* DynamicArray::allocateChunk(){
		PROFILE_FUNCTION();
		Chunk* chunk = (Chunk*)allocator->allocate();
		new (chunk) Chunk(this);
		return chunk;
	}

	void DynamicArray::pushChunks(uint32_t count){
		PROFILE_FUNCTION();
		for (int i=0; i<count; i++){
			pushBack(allocateChunk());
		}
	}

	DynamicArray::Chunk::Chunk(DynamicArray* owner) : owner{owner}{
		PROFILE_FUNCTION();
		next = nullptr;
	}
	
	void* DynamicArray::Chunk::get(uint32_t index){
		PROFILE_FUNCTION();
		RND_ASSERT(index < owner->elementPerChunk, "index out of range");

		char* ptr = ((char*)this) + sizeof(Chunk);
		uint32_t offset = owner->elementSize * index;

		return (void*)(ptr + index);
	}

	uint32_t DynamicArray::size(){
		PROFILE_FUNCTION();
		return elementPerChunk * chunkCount;
	}

	bool DynamicArray::empty(){
		PROFILE_FUNCTION();
		return chunkCount == 0;
	}

	void* DynamicArray::get(uint32_t index){
		PROFILE_FUNCTION();
		RND_ASSERT(index < elementPerChunk * chunkCount, "index out of range");
		
		uint32_t chunkID = ((float)index) / ((float)elementPerChunk);
		uint32_t localID = index - (chunkID * elementPerChunk);

		return getChunk(chunkID)->get(localID);
	}

	DynamicArray::Chunk* DynamicArray::getChunk(uint32_t id){
		PROFILE_FUNCTION();
		RND_ASSERT(id < chunkCount, "index out of range");
		// printf("%d\n", id);
		
		Chunk* chunk = begin;
		for (int i=0; i<id; i++){
			chunk = chunk->next;
		}

		return chunk;
	}
}