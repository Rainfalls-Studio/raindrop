#include "Core/String.hpp"
#include "Core/Debug/profiler.hpp"
#include <cstring>

namespace Raindrop::Core{
	String::String(Memory::Allocator& allocator, const char* chr) : _string(chr){
		RAINDROP_profile_function();
	}

	String::String(Memory::Allocator& allocator, const String &other) : _string(other._string){
		RAINDROP_profile_function();
	}

	String::String(Memory::Allocator& allocator, String &&other) : _string(other._string){
		RAINDROP_profile_function();
	}

	String::String(Memory::Allocator& allocator){
		RAINDROP_profile_function();
	}

	usize String::size() const{
		RAINDROP_profile_function();
		return (usize)_string.size();
	}

	String& String::operator=(const String &other){
		RAINDROP_profile_function();
		_string = other._string;
		return *this;
	}

	const char* String::str() const{
		RAINDROP_profile_function();
		return _string.c_str();
	}
			
	char* String::data(){
		RAINDROP_profile_function();
		return _string.data();
	}

	bool String::empty() const{
		RAINDROP_profile_function();
		return _string.empty();
	}

	void String::resize(usize size){
		RAINDROP_profile_function();
		_string.resize(size);
	}

	const char &String::operator[](usize i) const{
		RAINDROP_profile_function();
		return _string[i];
	}

	char &String::operator[](usize i){
		RAINDROP_profile_function();
		return _string[i];
	}

	void String::clear(){
		RAINDROP_profile_function();
		_string.clear();
	}

	void String::append(const char *str){
		RAINDROP_profile_function();
		_string.append(str);
	}

	void String::append(const String &other){
		RAINDROP_profile_function();
		_string.append(other._string);
	}

	void String::append(const char c){
		RAINDROP_profile_function();
		_string.append(&c);
	}

	bool String::operator==(const char *str) const{
		RAINDROP_profile_function();
		return _string == str;
	}

	bool String::operator==(const String &other) const{
		RAINDROP_profile_function();
		return other._string == _string;
	}

	ID64 String::hash64() const{
		std::hash<std::string> hash;
		return static_cast<ID64>(hash(_string));
	}
}

namespace Raindrop::Tools{
	template<>
	ID64 Tools::hash64<Core::String>(const Core::String& string){
		return string.hash64();
	}
}