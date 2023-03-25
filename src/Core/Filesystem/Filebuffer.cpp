#include "Filebuffer.hpp"
#include "../Debug/profiler.hpp"

namespace Raindrop::Core::Filesystem{

	#define check(stream, path){																		\
		if (!stream){																					\
			RAINDROP_log(ERROR, MEMORY, "failed to allocate enought memory for a filestream");			\
			return;																						\
		}																								\
																										\
		if (!stream->isOpen()){																			\
			RAINDROP_log(ERROR, MEMORY, "failed to open \"%s\" file");									\
			return;																						\
		}																								\
	}

	Filebuffer::Filebuffer(Memory::Allocator& allocator, Filestream& stream) : _content{allocator}{
		RAINDROP_profile_function();
	}

	Filebuffer::Filebuffer(Memory::Allocator& allocator, const File& file) : _content{allocator}{
		RAINDROP_profile_function();
		Filestream* stream = Memory::allocateNew<Filestream>(allocator, file, Filestream::OPEN_BEGIN | Filestream::OPEN_READ);
		check(stream, file.path().str());
		copy(stream);
		Memory::deallocateDelete(allocator, stream);
	}

	Filebuffer::Filebuffer(Memory::Allocator& allocator, const Path& path) : _content{allocator}{
		RAINDROP_profile_function();
		Filestream* stream = Memory::allocateNew<Filestream>(allocator, path, Filestream::OPEN_BEGIN | Filestream::OPEN_READ);
		check(stream, path.str());
		copy(stream);
		Memory::deallocateDelete(allocator, stream);
	}

	void Filebuffer::copy(Filestream* stream){
		RAINDROP_profile_function();
		usize size = 0;

		stream->setPos(0, stream->POS_END);
		size = stream->getPos();
		stream->setPos(0);
		_content.resize(size);

		stream->read(_content);
	}

	const String& Filebuffer::content() const{
		RAINDROP_profile_function();
		return _content;
	}

	char Filebuffer::operator[](usize i) const{
		RAINDROP_profile_function();
		return _content[i];
	}

	usize Filebuffer::size() const{
		RAINDROP_profile_function();
		return _content.size();
	}
}