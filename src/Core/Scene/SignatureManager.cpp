#include "SignatureManager.hpp"
#include "Scene.hpp"
#include <cstring>

namespace Raindrop::Core::Scene{
	SignatureManager::SignatureManager(Memory::Allocator& allocator, usize capacity) : _allocator{allocator}{
		RAINDROP_profile_function();
		_signatures = nullptr;
		_capacity = capacity;
		_signatures = Memory::allocateArray<Signature>(_allocator, capacity);
	}

	SignatureManager::~SignatureManager(){
		RAINDROP_profile_function();
		if (_signatures) _allocator.deallocate(_signatures);
		_signatures = nullptr;
	}

	void SignatureManager::setBit(usize i, usize b, bool state){
		RAINDROP_profile_function();
		Signature& sig = get(i);
		sig.set(b, state);
	}

	bool SignatureManager::getBit(usize i, usize b) const{
		RAINDROP_profile_function();
		const Signature& sig = get(i);
		return sig.get(i);
	}

	Signature& SignatureManager::get(usize i){
		RAINDROP_profile_function();
		RAINDROP_assert(i < _capacity);
		return _signatures[i];
	}

	const Signature& SignatureManager::get(usize i) const{
		RAINDROP_profile_function();
		RAINDROP_assert(i < _capacity);
		return _signatures[i];
	}

	Signature& SignatureManager::operator[](usize i){
		return get(i);
	}

	Signature::Signature(){
		clear();
	}

	Signature::Signature(uint64 sig){
		_sig = sig;
	}

	Signature::~Signature(){

	}

	void Signature::clear(){
		_sig = 0ULL;
	}

	void Signature::set(usize i, bool state){
		_sig ^= (-state ^ _sig) & (1ULL << (i % MAX_COMPONENT_COUNT));
	}

	bool Signature::get(usize i) const{
		return (_sig >> i % MAX_COMPONENT_COUNT) & 1ULL;
	}

	Signature Signature::operator&(const Signature& other) const{
		return Signature(_sig & other._sig);
	}

	Signature Signature::operator|(const Signature& other) const{
		return Signature(_sig | other._sig);
	}

	Signature Signature::operator^(const Signature& other) const{
		return Signature(_sig ^ other._sig);
	}

	Signature::operator bool() const{
		return _sig != 0ULL;
	}
	
	Signature& Signature::operator&=(const Signature& other){
		_sig &= other._sig;
		return *this;
	}

	Signature& Signature::operator|=(const Signature& other){
		_sig |= other._sig;
		return *this;
	}

	Signature& Signature::operator^=(const Signature& other){
		_sig ^= other._sig;
		return *this;
	}
}