#include "Core/Filesystem/DirectoryView.hpp"
#include "Core/Filesystem/Folder.hpp"
#include "Core/debug/profiler.hpp"

namespace Raindrop::Core::Filesystem{
	RAINDROP_API DirectoryView::DirectoryView(const Folder &folder, Memory::Allocator& allocator) : DirectoryView(folder.path(), allocator){
		RAINDROP_profile_function();
	}

	RAINDROP_API DirectoryView::DirectoryView(const Path &path, Memory::Allocator& allocator) : _path(path), _content(getFolderContent(allocator, path)){
		RAINDROP_profile_function();
	}

	RAINDROP_API DirectoryView::~DirectoryView(){
		RAINDROP_profile_function();
	}

	RAINDROP_API const Memory::Array<String>& DirectoryView::content() const{
		RAINDROP_profile_function();
		return _content;
	}

	RAINDROP_API const Path& DirectoryView::path() const{
		RAINDROP_profile_function();
		return _path;
	}

	RAINDROP_API usize DirectoryView::size() const{
		RAINDROP_profile_function();
		return _content.size();
	}
}