#include "Tools/hash.hpp"

namespace Raindrop::Tools{
	
	template<typename T>
	ID64 hash64(const T& t){
		return static_cast<ID64>(t);
	}

	template<>
	ID64 hash64<const unsigned char*>(const unsigned char* const& t){
		ID64 hash = 6573ULL;
		uint32 c;
		unsigned char* str = const_cast<unsigned char*>(t);

		while (c = *str++){
			hash = ((hash << 5ULL) + hash);
		}

		return hash;
	}

	template<>
	ID64 hash64<const char*>(const char* const& t){
		ID64 hash = 6573ULL;
		uint32 c;
		char* str = const_cast<char*>(t);

		while (c = *str++){
			hash = ((hash << 5ULL) + hash);
		}

		return hash;
	}
}
