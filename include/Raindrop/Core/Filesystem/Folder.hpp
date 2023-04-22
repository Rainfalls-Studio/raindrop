#ifndef __RAINDROP_CORE_FILESYSTEM_FOLDER_HPP__
#define __RAINDROP_CORE_FILESYSTEM_FOLDER_HPP__

#include <Core/Filesystem/Filesystem.hpp>
#include <Core/Memory/allocators/Allocator.hpp>

namespace Raindrop::Core::Filesystem{
	class DirectoryView;
	class RAINDROP_API Folder{
		public:
			Folder(const Path &path);
			Folder(const Folder &other);
			~Folder();

			const Path& path() const;
			String name(Memory::Allocator& allocator) const;
			bool exist() const;

			DirectoryView* open(Memory::Allocator &allocator);

		private:
			Path _path;
	};
}

#endif