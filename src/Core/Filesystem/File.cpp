#include "File.hpp"
#include "Filestream.hpp"
#include <stdio.h>
#include <filesystem>

#include "../debug/profiler.hpp"

namespace Raindrop::Core::Filesystem{
	File::File(const File& other) : _path(other._path){
		RAINDROP_profile_function();
	}

	File::File(const Path& path) : _path(path){
		RAINDROP_profile_function();
	}

	File::File(Memory::Allocator& allocator, const char* path) : _path(allocator, path){
		RAINDROP_profile_function();
	}

	File::~File(){
		RAINDROP_profile_function();
	}

	const usize File::size() const{
		RAINDROP_profile_function();
		return getSize(_path);
	}

	const Path &File::path() const{
		RAINDROP_profile_function();
		return _path;
	}

	bool File::exist() const{
		RAINDROP_profile_function();
		return Filesystem::exist(_path);
	}

	String File::name(Memory::Allocator& allocator) const{
		RAINDROP_profile_function();
		return getName(_path, allocator);
	}

	Extention File::extention(Memory::Allocator& allocator) const{
		RAINDROP_profile_function();
		return getExtention(_path, allocator);
	}

	Filestream* File::open(Memory::Allocator& allocator, Filestream::OpenModeFlag mode){
		RAINDROP_profile_function();
		return Memory::allocateNew<Filestream>(allocator, *this, mode);
	}

	bool File::copy(const String &path){
		RAINDROP_profile_function();
		return Filesystem::copy(_path, path);
	}

	bool File::move(const String &path){
		RAINDROP_profile_function();
		_path = path;
		return Filesystem::move(_path, path);
	}
	
	bool File::remove(){
		RAINDROP_profile_function();
		return Filesystem::remove(_path);
	}
}