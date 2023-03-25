#ifndef __RAINDROP_CORE_FILESYSTEM_FILEBUFFER_HPP__
#define __RAINDROP_CORE_FILESYSTEM_FILEBUFFER_HPP__

#include "Filesystem.hpp"
#include "Filestream.hpp"
#include "File.hpp"
#include "../Memory/allocators/Allocator.hpp"
#include "../Memory/containers/Array.hpp"

namespace Raindrop::Core::Filesystem{
	class Filebuffer{
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