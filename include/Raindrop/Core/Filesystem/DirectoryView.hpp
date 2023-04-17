#ifndef __RAINDROP_CORE_FILESYSTEM_DIRECTORY_VIEW_HPP__
#define __RAINDROP_CORE_FILESYSTEM_DIRECTORY_VIEW_HPP__

#include <Core/Filesystem/Filesystem.hpp>

namespace Raindrop::Core::Filesystem{
	class Folder;
	class DirectoryView{
		public:
			DirectoryView(const Folder &folder, Memory::Allocator& allocator);
			DirectoryView(const Path &path, Memory::Allocator& allocator);
			~DirectoryView();

			const Memory::Array<String>& content() const;
			const Path& path() const;
			usize size() const;

		private:
			Path _path;
			Memory::Array<String> _content;
	};
}

#endif