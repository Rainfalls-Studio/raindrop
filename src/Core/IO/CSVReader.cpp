#include "Core/IO/CSVReader.hpp"
#include "Core/Debug/debug.hpp"
#include "Core/Debug/profiler.hpp"

namespace Raindrop::Core::IO{
	RAINDROP_API CSVReader::CSVReader(Memory::Allocator &allocator, const Filesystem::Filebuffer &buffer) : _allocator{&allocator}, _content{allocator}{
		RAINDROP_profile_function();
		_columns = 0;
		_rows = 0;

		read(buffer);
	}

	RAINDROP_API CSVReader::~CSVReader(){
		RAINDROP_profile_function();
		usize size = _content.size();
		for (usize i=0; i<size; i++){
			auto& str = _content[i];
			_allocator->deallocate(str);
		}
	}

	RAINDROP_API void CSVReader::read(const Filesystem::Filebuffer &buffer){
		RAINDROP_profile_function();
		_columns = getCommaPerLine(buffer) + 1;
		_rows = getLineCount(buffer);

		usize size = _columns * _rows;
		_content.resize(size);

		usize curr = 0;
		for (usize i=0; i<size; i++){
			usize lastPos = curr;

			while (buffer[curr] != ';' && buffer[curr] != '\n'){
				curr++;
			}

			String* str = Memory::allocateNew<String, Memory::Allocator&>(*_allocator, *_allocator);
			_content[i] = str;

			usize wordSize = curr - lastPos;
			str->resize(wordSize);
			
			usize wordI = 0;
			while (lastPos != curr){
				(*str)[wordI] = buffer[lastPos];

				wordI++;
				lastPos++;
			}

			curr++;
		}
	}

	RAINDROP_API usize CSVReader::getCommaPerLine(const Filesystem::Filebuffer &buffer){
		RAINDROP_profile_function();
		usize count = 0;
		usize size = buffer.size();

		for (usize i=0; i<size; i++){
			char c = buffer[i];

			if (c == ';'){
				count++;
			} else if (c == '\n'){
				break;
			}
		}

		return count;
	}
	
	RAINDROP_API usize CSVReader::getLineCount(const Filesystem::Filebuffer &buffer){
		RAINDROP_profile_function();
		usize count = 0;
		usize size = buffer.size();

		for (usize i=0; i<size; i++){
			char c = buffer[i];
			if (c == '\n') count++;
		}

		return count;
	}

	RAINDROP_API uint32 CSVReader::columns() const{
		RAINDROP_profile_function();
		return _columns;
	}

	RAINDROP_API uint32 CSVReader::rows() const{
		RAINDROP_profile_function();
		return _rows;
	}

	RAINDROP_API const String& CSVReader::get(uint32 column, uint32 row) const{
		RAINDROP_profile_function();
		return *_content[(column * _columns) + row];
	}

	RAINDROP_API const String& CSVReader::operator()(uint32 column, uint32 row) const{
		RAINDROP_profile_function();
		return this->get(column, row);
	}
}