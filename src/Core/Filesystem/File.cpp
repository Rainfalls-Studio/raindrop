#include "Core/Filesystem/File.hpp"
#include "Core/Filesystem/Filestream.hpp"
#include "Core/Debug/profiler.hpp"
#include <stdio.h>
#include <filesystem>


namespace Raindrop::Core::Filesystem{
	RAINDROP_API File::File(const File& other) : _path(other._path){
		RAINDROP_profile_function();
	}

	RAINDROP_API File::File(const Path& path) : _path(path){
		RAINDROP_profile_function();
	}

	RAINDROP_API File::File(Memory::Allocator& allocator, const char* path) : _path(allocator, path){
		RAINDROP_profile_function();
	}

	RAINDROP_API File::~File(){
		RAINDROP_profile_function();
	}

	RAINDROP_API const usize File::size() const{
		RAINDROP_profile_function();
		return getSize(_path);
	}

	RAINDROP_API const Path &File::path() const{
		RAINDROP_profile_function();
		return _path;
	}

	RAINDROP_API bool File::exist() const{
		RAINDROP_profile_function();
		return Filesystem::exist(_path);
	}

	RAINDROP_API String File::name(Memory::Allocator& allocator) const{
		RAINDROP_profile_function();
		return getName(_path, allocator);
	}

	RAINDROP_API Extention File::extention(Memory::Allocator& allocator) const{
		RAINDROP_profile_function();
		return getExtention(_path, allocator);
	}

	RAINDROP_API Filestream* File::open(Memory::Allocator& allocator, Filestream::OpenModeFlag mode){
		RAINDROP_profile_function();
		return Memory::allocateNew<Filestream>(allocator, *this, mode);
	}

	RAINDROP_API bool File::copy(const String &path){
		RAINDROP_profile_function();
		return Filesystem::copy(_path, path);
	}

	RAINDROP_API bool File::move(const String &path){
		RAINDROP_profile_function();
		_path = path;
		return Filesystem::move(_path, path);
	}
	
	RAINDROP_API bool File::remove(){
		RAINDROP_profile_function();
		return Filesystem::remove(_path);
	}
}