#include "Folder.hpp"
#include "DirectoryView.hpp"
#include "../Debug/profiler.hpp"

namespace Raindrop::Core::Filesystem{
	Folder::Folder(const Path &path) : _path(path){
		RAINDROP_profile_function();
	}

	Folder::Folder(const Folder &other) : _path(other._path){
		RAINDROP_profile_function();
	}
	Folder::~Folder(){}

	const Path& Folder::path() const{
		RAINDROP_profile_function();
		return _path;
	}

	String Folder::name(Memory::Allocator& allocator) const{
		RAINDROP_profile_function();
		return getName(_path, allocator);
	}

	bool Folder::exist() const{
		RAINDROP_profile_function();
		return Filesystem::exist(_path);
	}

	DirectoryView* Folder::open(Memory::Allocator &allocator){
		RAINDROP_profile_function();
		return Memory::allocateNew<DirectoryView, const Folder&, Memory::Allocator&>(allocator, *this, allocator);
	}
}