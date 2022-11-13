#include "memory/ForwardLinkedList.hpp"
#include "memory/ChunkAllocator.hpp"
#include "debug/Instrumentor.hpp"

namespace rnd::memory{
	void ForwardLinkedList::init(Allocator* allocator, uint32_t elementSize){
		PROFILE_FUNCTION();
		customAllocator = true;
		begin = nullptr;
		end = nullptr;
		this->allocator = allocator;
		this->elementSize = elementSize + sizeof(Node);
	}

	void ForwardLinkedList::init(uint32_t elementSize){
		PROFILE_FUNCTION();
		this->elementSize = elementSize + sizeof(Node);
		begin = nullptr;
		end = nullptr;
		allocator = createAllocator();
	}

	void ForwardLinkedList::shutdown(){
		PROFILE_FUNCTION();

		if (!customAllocator){
			allocator->shutdown();
			free(allocator);
		}

		allocator = nullptr;
		begin = nullptr;
		end = nullptr;
		elementSize = 0;
		nodeCount = 0;
	}

	void ForwardLinkedList::push(void* ptr){
		PROFILE_FUNCTION();

		Node* node = (Node*)allocator->allocate();
		memcpy(node->get(), ptr, elementSize - sizeof(Node));

		node->next = nullptr;
		if (begin){
			begin->next = node;
		} else {
			end = node;
		}

		begin = node;
		nodeCount++;
	}

	void ForwardLinkedList::pop(){
		PROFILE_FUNCTION();
		if (!end) return;

		Node* node = end;
		end = end->next;
		allocator->deallocate(node);
		nodeCount--;
	}

	void* ForwardLinkedList::front(){
		PROFILE_FUNCTION();
		return end;
	}

	void* ForwardLinkedList::back(){
		PROFILE_FUNCTION();
		return begin;
	}

	void ForwardLinkedList::clear(){
		PROFILE_FUNCTION();
		end = nullptr;
		begin = nullptr;
		nodeCount = 0;
	}

	uint32_t ForwardLinkedList::size(){
		PROFILE_FUNCTION();
		return nodeCount;
	}

	bool ForwardLinkedList::empty(){
		PROFILE_FUNCTION();
		return nodeCount == 0;
	}
	
	Allocator* ForwardLinkedList::createAllocator(){
		PROFILE_FUNCTION();
		ChunkAllocator* allocator = (ChunkAllocator*)malloc(sizeof(ChunkAllocator));
		allocator->init(elementSize, 0, 100);
		return allocator;
	}

	void* ForwardLinkedList::Node::get(){
		return ((char*)this) + sizeof(Node);
	}

}