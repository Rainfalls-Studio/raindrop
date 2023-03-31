#ifndef __RAINDROP_CORE_MEMORY_DYNAMIC_BITSET_HPP__
#define __RAINDROP_CORE_MEMORY_DYNAMIC_BITSET_HPP__

#include "Core/core.hpp"
#include "Core/common.hpp"
#include "Core/Memory/allocators/Allocator.hpp"
#include "Core/Maths/Maths.hpp"
#include <cstring>

namespace Raindrop::Core::Memory{
	class DynamicBitset{
		public:
			DynamicBitset(Allocator& allocator, usize lenght);
			~DynamicBitset();

			void set(uint32 i, bool statut = true);

			bool get(uint32 i) const;
			bool operator[](uint32 i) const;

			void resize(usize newLenght);

		private:
			Allocator& _allocator;
			usize _lenght;
			uint8* _bytes;

	};

	DynamicBitset::DynamicBitset(Allocator& allocator, usize lenght) : _allocator{allocator}{
		_bytes = nullptr;
		resize(lenght);
	}

	DynamicBitset::~DynamicBitset(){
		if (_bytes) _allocator.deallocate(_bytes);
	}

	void DynamicBitset::set(uint32 i, bool statut){
		uint8& byte = _bytes[i / 8];
		byte ^= (-statut ^ byte) & (1UL << (i % 8));
	}
			
	bool DynamicBitset::get(uint32 i) const{
		uint8 byte = _bytes[i / 8];
		return (byte >> i % 8) & 1U;
	}
	
	bool DynamicBitset::operator[](uint32 i) const{
		return get(i);
	}

	void DynamicBitset::resize(usize newLenght){
		uint32 byteCount = _lenght / 8;

		uint8* newBytes = reinterpret_cast<uint8*>(_allocator.allocate(byteCount, alignof(uint8)));
		if (!newBytes) return;

		for (uint32 i=0; i<byteCount; i++){
			newBytes[i] = 0;
		}

		if (_bytes){
			memcpy(newBytes, _bytes, Maths::min(newLenght, _lenght));
			_allocator.deallocate(_bytes);
		}

		_bytes = newBytes;
		_lenght = newLenght;
	}
}



#endif