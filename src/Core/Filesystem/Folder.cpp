#include "Core/Filesystem/Folder.hpp"
#include "Core/Filesystem/DirectoryView.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::Filesystem{
	RAINDROP_API Folder::Folder(const Path &path) : _path(path){
		RAINDROP_profile_function();
	}

	RAINDROP_API Folder::Folder(const Folder &other) : _path(other._path){
		RAINDROP_profile_function();
	}
	RAINDROP_API Folder::~Folder(){}

	RAINDROP_API const Path& Folder::path() const{
		RAINDROP_profile_function();
		return _path;
	}

	RAINDROP_API String Folder::name(Memory::Allocator& allocator) const{
		RAINDROP_profile_function();
		return getName(_path, allocator);
	}

	RAINDROP_API bool Folder::exist() const{
		RAINDROP_profile_function();
		return Filesystem::exist(_path);
	}

	RAINDROP_API DirectoryView* Folder::open(Memory::Allocator &allocator){
		RAINDROP_profile_function();
		return Memory::allocateNew<DirectoryView, const Folder&, Memory::Allocator&>(allocator, *this, allocator);
	}
}