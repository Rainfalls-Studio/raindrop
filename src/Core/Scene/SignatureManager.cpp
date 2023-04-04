#include "Core/Scene/SignatureManager.hpp"
#include "Core/Scene/Scene.hpp"
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
		if (!inRange(i)) throw std::out_of_range("the ID of the signature is out of range");
		Signature& sig = get(i);
		sig.set(b, state);
	}

	bool SignatureManager::getBit(usize i, usize b) const{
		RAINDROP_profile_function();
		if (!inRange(i)) throw std::out_of_range("the ID of the signature is out of range");
		const Signature& sig = get(i);
		return sig.get(b);
	}

	Signature& SignatureManager::get(usize i){
		RAINDROP_profile_function();
		if (!inRange(i)) throw std::out_of_range("the ID of the signature is out of range");
		return _signatures[i];
	}

	const Signature& SignatureManager::get(usize i) const{
		RAINDROP_profile_function();
		if (!inRange(i)) throw std::out_of_range("the ID of the signature is out of range");
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
		if (!inRange(i)) throw std::out_of_range("the ID of the bit in the signature is out of range");
		_sig ^= (-state ^ _sig) & (1ULL << i);
	}

	bool Signature::get(usize i) const{
		if (!inRange(i)) throw std::out_of_range("the ID of the bit in the signature is out of range");
		return (_sig >> i) & 1ULL;
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
	
	usize SignatureManager::capacity() const{
		return _capacity;
	}

	usize SignatureManager::bitesPerSignature(){
		return MAX_COMPONENT_COUNT;
	}
}