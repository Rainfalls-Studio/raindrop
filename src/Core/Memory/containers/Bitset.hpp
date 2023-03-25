#ifndef __RAINDROP_CORE_MEMORY_BITSET_HPP__
#define __RAINDROP_CORE_MEMORY_BITSET_HPP__

#include "../../core.hpp"
#include "../../common.hpp"

namespace Raindrop::Core::Memory{
	template<uint32 lenght>
	class Bitset{
		public:
			Bitset();
			~Bitset();

			void set(uint32 i, bool statut = true);

			bool get(uint32 i) const;
			bool operator[](uint32 i) const;

		private:
			uint8 _bytes[(lenght / 8) + 1];

	};

	template<uint32 lenght>
	Bitset<lenght>::Bitset(){
		uint32 byteCount = lenght / 8;
		for (uint32 i=0; i<byteCount; i++){
			_bytes[i] = 0;
		}
	}

	template<uint32 lenght>
	Bitset<lenght>::~Bitset(){}

	template<uint32 lenght>
	void Bitset<lenght>::set(uint32 i, bool statut){
		uint8& byte = _bytes[i / 8];
		byte ^= (-statut ^ byte) & (1UL << (i % 8));
	}
			
	template<uint32 lenght>
	bool Bitset<lenght>::get(uint32 i) const{
		uint8 byte = _bytes[i / 8];
		return (byte >> i % 8) & 1U;
	}
	
	template<uint32 lenght>
	bool Bitset<lenght>::operator[](uint32 i) const{
		return get(i);
	}
}



#endif