#include "memory/ChunkAllocator.hpp"
#include "debug/Instrumentor.hpp"
#include <math.h>

namespace rnd::memory{
	void ChunkAllocator::init(uint32_t dataSize, uint32_t size, uint32_t instancePerChunk){
		PROFILE_FUNCTION();
		new (this) (ChunkAllocator);

		begin = nullptr;
		buffers = nullptr;

		this->dataSize = rnd_max(dataSize, sizeof(Buffer));
		this->instancePerChunk = instancePerChunk;

		pthread_mutex_init(&lock, nullptr);
		int chunkCount = std::ceil((float)size / (float)instancePerChunk);
		pushChunks(chunkCount);
	}

	void ChunkAllocator::shutdown(){
		PROFILE_FUNCTION();
		freeChunks();
		dataSize = 0;
		instancePerChunk = 0;
	}

	ChunkAllocator::Chunk* ChunkAllocator::allocateChunk(){
		PROFILE_FUNCTION();
		Chunk* chunk = (Chunk*)malloc(sizeof(Chunk) + (dataSize * instancePerChunk));

		if (chunk == nullptr){
			RND_ERR("malloc error", "failed to allocate a chunk allocator's chunk");
			exit(EXIT_FAILURE);
		}

		return chunk;
	}

	void ChunkAllocator::pushChunk(){
		PROFILE_FUNCTION();
		Chunk* chunk = allocateChunk();

		chunk->next = begin;
		begin = chunk;

		char* buffer = (char*)chunk->get();
		for (int i=0; i<instancePerChunk; i++){
			pushBuffer((Buffer*)buffer);
			buffer += dataSize;
		}
	}

	void ChunkAllocator::pushChunks(uint32_t count){
		PROFILE_FUNCTION();

		for (int i=0; i<count; i++){
			pushChunk();
		}
	}

	void ChunkAllocator::pushBuffer(Buffer* buffer){
		PROFILE_FUNCTION();
		buffer->next = buffers;
		buffers = buffer;
	}
	void* ChunkAllocator::allocate(){
		PROFILE_FUNCTION();
		pthread_mutex_lock(&lock);
		if (buffers == nullptr) pushChunk();
		
		Buffer* buffer = buffers;
		buffers = buffers->next;
		pthread_mutex_unlock(&lock);

		return buffer;
	}

	void ChunkAllocator::deallocate(void* ptr){
		PROFILE_FUNCTION();
		pthread_mutex_lock(&lock);
		pushBuffer((Buffer*)ptr);
		pthread_mutex_unlock(&lock);
	}

	ChunkAllocator::Buffer* ChunkAllocator::Chunk::get(){
		PROFILE_FUNCTION();
		return (Buffer*)((char*)this + sizeof(Chunk));
	}

	void ChunkAllocator::clear(){
		PROFILE_FUNCTION();
		freeChunks();
	}

	void ChunkAllocator::freeChunks(){
		PROFILE_FUNCTION();
		Chunk* chunk = begin;

		while (chunk){
			Chunk* current = chunk;
			chunk = chunk->next;
			free(current);
		}

		begin = nullptr;
		buffers = nullptr;
	}
}