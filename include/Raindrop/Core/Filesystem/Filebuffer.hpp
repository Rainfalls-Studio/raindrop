#ifndef __RAINDROP_CORE_FILESYSTEM_FILEBUFFER_HPP__
#define __RAINDROP_CORE_FILESYSTEM_FILEBUFFER_HPP__

#include <Core/Filesystem/Filesystem.hpp>
#include <Core/Filesystem/Filestream.hpp>
#include <Core/Filesystem/File.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include <Core/Memory/containers/Array.hpp>

namespace Raindrop::Core::Filesystem{
	class RAINDROP_API Filebuffer{
		public:
			Filebuffer(Memory::Allocator& allocator, Filestream& stream);
			Filebuffer(Memory::Allocator& allocator, const File& file);
			Filebuffer(Memory::Allocator& allocator, const Path& path);
			~Filebuffer() = default;

			const String& content() const;
			char operator[](usize i) const;
			usize size() const;

		private:
			String _content;

			void copy(Filestream* stream);

	};
}

#endif