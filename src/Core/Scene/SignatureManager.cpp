#include <Core/Scene/SignatureManager.hpp>
#include <Core/Scene/Scene.hpp>
#include <Core/Debug/profiler.hpp>
#include <cstring>

namespace Raindrop::Core::Scene{
	RAINDROP_API SignatureManager::SignatureManager(usize capacity){
		RAINDROP_profile_function();
		_capacity = capacity;
		_signatures = std::make_unique<Signature[]>(capacity);
	}

	RAINDROP_API SignatureManager::~SignatureManager(){
		RAINDROP_profile_function();
		_signatures.release();
	}

	RAINDROP_API void SignatureManager::setBit(usize i, usize b, bool state){
		RAINDROP_profile_function();
		if (!inRange(i)) throw std::out_of_range("the ID of the signature is out of range");
		Signature& sig = get(i);
		sig.set(b, state);
	}

	RAINDROP_API bool SignatureManager::getBit(usize i, usize b) const{
		RAINDROP_profile_function();
		if (!inRange(i)) throw std::out_of_range("the ID of the signature is out of range");
		const Signature& sig = get(i);
		return sig.get(b);
	}

	RAINDROP_API Signature& SignatureManager::get(usize i){
		RAINDROP_profile_function();
		if (!inRange(i)) throw std::out_of_range("the ID of the signature is out of range");
		return _signatures[i];
	}

	RAINDROP_API const Signature& SignatureManager::get(usize i) const{
		RAINDROP_profile_function();
		if (!inRange(i)) throw std::out_of_range("the ID of the signature is out of range");
		return _signatures[i];
	}

	RAINDROP_API Signature& SignatureManager::operator[](usize i){
		return get(i);
	}

	RAINDROP_API Signature::Signature(){
		clear();
	}

	RAINDROP_API Signature::Signature(uint64 sig){
		_sig = sig;
	}

	RAINDROP_API Signature::~Signature(){

	}

	RAINDROP_API void Signature::clear(){
		_sig = 0ULL;
	}

	RAINDROP_API void Signature::set(usize i, bool state){
		if (!inRange(i)) throw std::out_of_range("the ID of the bit in the signature is out of range");
		_sig ^= (-state ^ _sig) & (1ULL << i);
	}

	RAINDROP_API bool Signature::get(usize i) const{
		if (!inRange(i)) throw std::out_of_range("the ID of the bit in the signature is out of range");
		return (_sig >> i) & 1ULL;
	}

	RAINDROP_API Signature Signature::operator&(const Signature& other) const{
		return Signature(_sig & other._sig);
	}

	RAINDROP_API Signature Signature::operator|(const Signature& other) const{
		return Signature(_sig | other._sig);
	}

	RAINDROP_API Signature Signature::operator^(const Signature& other) const{
		return Signature(_sig ^ other._sig);
	}

	RAINDROP_API Signature::operator bool() const{
		return _sig != 0ULL;
	}
	
	RAINDROP_API Signature& Signature::operator&=(const Signature& other){
		_sig &= other._sig;
		return *this;
	}

	RAINDROP_API Signature& Signature::operator|=(const Signature& other){
		_sig |= other._sig;
		return *this;
	}

	RAINDROP_API Signature& Signature::operator^=(const Signature& other){
		_sig ^= other._sig;
		return *this;
	}
	
	RAINDROP_API usize SignatureManager::capacity() const{
		return _capacity;
	}

	RAINDROP_API usize SignatureManager::bitesPerSignature(){
		return MAX_COMPONENT_COUNT;
	}
}