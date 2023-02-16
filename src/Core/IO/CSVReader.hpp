#ifndef __RAINDROP_CORE_IO_CSV_HPP__
#define __RAINDROP_CORE_IO_CSV_HPP__

#include "../Filesystem/Filebuffer.hpp"
#include "../Memory/allocators/Allocator.hpp"
#include "../Memory/containers/Array.hpp"
#include "../String.hpp"

namespace Raindrop::Core::IO{
	class CSVReader{
		public:
			CSVReader(Memory::Allocator &allocator, const Filesystem::Filebuffer &buffer);
			~CSVReader();

			uint32 columns() const;
			uint32 rows() const;

			const String& get(uint32 column, uint32 row) const;
			const String& operator()(uint32 column, uint32 row) const;

		private:
			Memory::Allocator* _allocator;
			Memory::Array<String*> _content;

			uint32 _columns;
			uint32 _rows;

			void read(const Filesystem::Filebuffer &buffer);
			static usize getCommaPerLine(const Filesystem::Filebuffer &buffer);
			static usize getLineCount(const Filesystem::Filebuffer &buffer);
	};
}

#endif