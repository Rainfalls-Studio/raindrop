#include "Core/String.hpp"
#include "Core/Debug/profiler.hpp"
#include <cstring>

namespace Raindrop::Core{
	RAINDROP_API String::String(Memory::Allocator& allocator, const char* chr) : _string(chr){
		RAINDROP_profile_function();
	}

	RAINDROP_API String::String(Memory::Allocator& allocator, const String &other) : _string(other._string){
		RAINDROP_profile_function();
	}

	RAINDROP_API String::String(Memory::Allocator& allocator, String &&other) : _string(other._string){
		RAINDROP_profile_function();
	}

	RAINDROP_API String::String(Memory::Allocator& allocator){
		RAINDROP_profile_function();
	}

	RAINDROP_API usize String::size() const{
		RAINDROP_profile_function();
		return (usize)_string.size();
	}

	RAINDROP_API String& String::operator=(const String &other){
		RAINDROP_profile_function();
		_string = other._string;
		return *this;
	}

	RAINDROP_API const char* String::str() const{
		RAINDROP_profile_function();
		return _string.c_str();
	}
			
	RAINDROP_API char* String::data(){
		RAINDROP_profile_function();
		return _string.data();
	}

	RAINDROP_API bool String::empty() const{
		RAINDROP_profile_function();
		return _string.empty();
	}

	RAINDROP_API void String::resize(usize size){
		RAINDROP_profile_function();
		_string.resize(size);
	}

	RAINDROP_API const char &String::operator[](usize i) const{
		RAINDROP_profile_function();
		return _string[i];
	}

	RAINDROP_API char &String::operator[](usize i){
		RAINDROP_profile_function();
		return _string[i];
	}

	RAINDROP_API void String::clear(){
		RAINDROP_profile_function();
		_string.clear();
	}

	RAINDROP_API void String::append(const char *str){
		RAINDROP_profile_function();
		_string.append(str);
	}

	RAINDROP_API void String::append(const String &other){
		RAINDROP_profile_function();
		_string.append(other._string);
	}

	RAINDROP_API void String::append(const char c){
		RAINDROP_profile_function();
		_string.append(&c);
	}

	RAINDROP_API bool String::operator==(const char *str) const{
		RAINDROP_profile_function();
		return _string == str;
	}

	RAINDROP_API bool String::operator==(const String &other) const{
		RAINDROP_profile_function();
		return other._string == _string;
	}

	RAINDROP_API ID64 String::hash64() const{
		std::hash<std::string> hash;
		return static_cast<ID64>(hash(_string));
	}

	RAINDROP_API String& String::operator=(const char* str){
		_string = str;
		return *this;
	}
}

namespace Raindrop::Tools{
	template<>
	ID64 Tools::hash64<Core::String>(const Core::String& string){
		return string.hash64();
	}
}