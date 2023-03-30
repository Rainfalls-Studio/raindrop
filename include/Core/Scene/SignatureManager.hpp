#ifndef __RAINDROP_CORE_SCENE_SIGNATURE_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_SIGNATURE_MANAGER_HPP__

#include "../core.hpp"
#include "../common.hpp"
#include "../Memory/allocators/Allocator.hpp"

namespace Raindrop::Core::Scene{
	class Signature{
		public:
			Signature();
			~Signature();

			void set(usize i, bool state);
			bool get(usize i) const;
			void clear();

			Signature operator&(const Signature& other) const;
			Signature operator|(const Signature& other) const;
			Signature operator^(const Signature& other) const;
			Signature& operator&=(const Signature& other);
			Signature& operator|=(const Signature& other);
			Signature& operator^=(const Signature& other);
			operator bool() const;

		private:
			uint64 _sig;
			
			Signature(uint64 sig);
	};
	class SignatureManager{
		public:
			SignatureManager(Memory::Allocator& allocator, usize capacity);
			~SignatureManager();

			Signature& get(usize i);
			const Signature& get(usize i) const;
			Signature& operator[](usize i);

			void setBit(usize i, usize b, bool state);
			bool getBit(usize i, usize b) const;
		
		private:
			Memory::Allocator& _allocator;
			Signature* _signatures;
			usize _capacity;
	};
}

#endif