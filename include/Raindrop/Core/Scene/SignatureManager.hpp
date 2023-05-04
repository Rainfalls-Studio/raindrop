#ifndef __RAINDROP_CORE_SCENE_SIGNATURE_MANAGER_HPP__
#define __RAINDROP_CORE_SCENE_SIGNATURE_MANAGER_HPP__

#include <Core/Scene/common.hpp>
#include <Core/Memory/memory.hpp>

namespace Raindrop::Core::Scene{
	class RAINDROP_API Signature{
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
			inline static bool inRange(const usize& i){
				return i < (sizeof(Signature::_sig) * 8);
			}
	};
	class RAINDROP_API SignatureManager{
		public:
			SignatureManager(usize capacity);
			~SignatureManager();

			Signature& get(usize i);
			const Signature& get(usize i) const;
			Signature& operator[](usize i);

			void setBit(usize i, usize b, bool state);
			bool getBit(usize i, usize b) const;

			usize capacity() const;
			static usize bitesPerSignature();
		
		private:
			std::unique_ptr<Signature[]> _signatures;
			usize _capacity;

			inline bool inRange(const usize& i) const{
				return i < _capacity;
			}
	};
}

#endif