#include "FreelistAllocator.hpp"
#include "../memory.hpp"
#include "../../Debug/debug.hpp"
#include "../../Debug/profiler.hpp"

namespace Raindrop::Core::Memory{
	FreelistAllocator::FreeBlock* FreelistAllocator::push(void* ptr, usize size){
		RAINDROP_profile_function();
		FreeBlock* node = (FreeBlock*)ptr;

		node->_size = size;
		node->_next = _begin;
		_begin = node;

		_size++;
		return node;
	}

	FreelistAllocator::FreelistAllocator(usize size, void* start) : Allocator(size, start){
		RAINDROP_profile_function();
		_begin = nullptr;
		_size = 0;
		push(start, size);
	}

	FreelistAllocator::~FreelistAllocator(){
		RAINDROP_profile_function();
		_begin = nullptr;
		_size = 0;
	}

	void* FreelistAllocator::allocate(usize size, uint8 alignement){
		RAINDROP_profile_function();
		size += sizeof(Header);
		FreeBlock* bestNode = popBestNode(size);
		if (bestNode == nullptr) return nullptr;

		Header* header = (Header*)bestNode;
		header->_size = size;

		void* ptr = header+1;

		allocationCount++;
		usedMemory+=size;

		return ptr;
	}

	void FreelistAllocator::deallocate(void* p){
		RAINDROP_profile_function();
		Header* header = (Header*)p - 1;

		allocationCount--;
		usedMemory -= header->_size;

		if (validateNode(header)){
			push(header, header->_size);
		}
	}

	FreelistAllocator::FreeBlock* FreelistAllocator::popBestNode(usize size){
		RAINDROP_profile_function();
		FreeBlock* curr = _begin;
		FreeBlock* before = nullptr;
		FreeBlock* beforeBestNode = nullptr;
		FreeBlock* bestNode = nullptr;

		while (curr){
			if (curr->_size < size){
				before = curr;
				curr = curr->_next;
				continue;
			}

			if (bestNode){
				if (bestNode->_size > curr->_size){
					beforeBestNode = before;
					bestNode = curr;
				}
			} else {
				beforeBestNode = before;
				bestNode = curr;
			}

			if (curr->_size == size){
				beforeBestNode = before;
				bestNode = curr;
				break;
			}

			before = curr;
			curr = curr->_next;
		}

		if (!bestNode){
			RAINDROP_log(ERROR, MEMORY, "not enought memory to allocate ! Cannot find a block with enought memory (required size : %d)", size);
			return nullptr;
		}

		if (bestNode == _begin){
			_begin = nullptr;
		}

		if (beforeBestNode){
			beforeBestNode->_next = bestNode->_next;
		}

		push((char*)bestNode + size, bestNode->_size - size);

		_size--;
		return bestNode;
	}

	void FreelistAllocator::clear(){
		RAINDROP_profile_function();
		_begin = nullptr;
		_size = 0;
		allocationCount = 0;
		usedMemory = 0;
		push(start, size);
	}

	bool FreelistAllocator::validateNode(Header* header){
		RAINDROP_profile_function();
		FreeBlock* curr = _begin;
		bool valid = true;

		while (curr){
			if ((char*)curr + curr->_size == (char*)header){
				curr->_size += header->_size;
				valid = false;
				break;
			}

			curr = curr->_next;
		}

		if ((char*)_begin + _begin->_size == (char*)_begin->_next){
			_begin->_size += _begin->_next->_size;
			_begin->_next = _begin->_next->_next;
			_size--;
		}

		return valid;
	}
}