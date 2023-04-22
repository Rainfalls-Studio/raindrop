#include <Core/Filesystem/Filestream.hpp>
#include <Core/Filesystem/File.hpp>
#include <Core/Filesystem/Filesystem.hpp>
#include <Core/Debug/profiler.hpp>

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

namespace Raindrop::Core::Filesystem{
	RAINDROP_API Filestream::Filestream(){
		RAINDROP_profile_function();
		_file = nullptr;
		_openMode = 0;
	}

	RAINDROP_API Filestream::Filestream(const File& file, OpenModeFlag openMode){
		RAINDROP_profile_function();
		_file = nullptr;
		open(file, openMode);
	}

	RAINDROP_API Filestream::Filestream(const Path& path, OpenModeFlag openMode){
		RAINDROP_profile_function();
		_file = nullptr;
		open(path, openMode);
	}

	RAINDROP_API Filestream::~Filestream(){
		RAINDROP_profile_function();
		close();
	}

	RAINDROP_API bool Filestream::open(const File& file, OpenModeFlag openMode){
		RAINDROP_profile_function();
		return open(file.path(), openMode);
	}

	RAINDROP_API bool Filestream::open(const Path& path, OpenModeFlag openMode){
		RAINDROP_profile_function();
		return open(path.str(), openMode);
	}

	RAINDROP_API void openModeToStrMode(Filestream::OpenModeFlag mode, char* str){
		RAINDROP_profile_function();
		uint8 i = 0;

		if (mode & Filestream::OPEN_WRITE){
			str[i] = 'w';
			i++;
			if (mode & Filestream::OPEN_BINARY){
				str[i] = 'b';
				i++;
			}

			if (mode & Filestream::OPEN_READ){
				str[i] = '+';
				i++;
			}
			
		} else{
			if (mode & Filestream::OPEN_READ){
				str[i] = 'r';
			} else if (mode & Filestream::OPEN_APPEND){
				str[i] = 'a';
			}

			i++;

			if (mode & Filestream::OPEN_BINARY){
				str[i] = 'b';
				i++;
			}

			if (mode & Filestream::OPEN_BEGIN){
				str[i] = '+';
				i++;
			}
		}

		str[i] = '\0';
	}


	RAINDROP_API bool Filestream::open(const char* path, OpenModeFlag openMode){
		RAINDROP_profile_function();
		_openMode = openMode;

		char mode[4];
		openModeToStrMode(openMode, mode);
 
		FILE* f = nullptr;

		if (!_file){
			f = fopen(path, mode);
		} else {
			f = freopen(path, mode, (FILE*)_file);
		}

		if (f == nullptr){
			RAINDROP_log(ERROR, IO, "failed to open a file : \"%s\" with %s mode : %s", path, mode, strerror(errno));
			return false;
		}

		_file = f;
		return true;
	}

	RAINDROP_API void Filestream::close(){
		RAINDROP_profile_function();
		if (_file){
			fclose((FILE*)_file);
			_file = nullptr;
			_openMode = 0;
		}
	}

	RAINDROP_API void Filestream::flush(){
		RAINDROP_profile_function();
		fflush((FILE*)_file);
	}

	RAINDROP_API unsigned char Filestream::readChar(bool *end) const{
		RAINDROP_profile_function();
		RAINDROP_assert(_openMode & OPEN_READ);
		int out = fgetc((FILE*)_file);

		if (out == EOF){
			RAINDROP_log(WARNING, IO, "cannot read further, hit the end of the file : %s", strerror(errno));

			if (end){
				*end = true;
			}

			return -1;
		}

		if (end){
			*end = false;
		}

		return (unsigned char)out;
	}

	RAINDROP_API String Filestream::readString(Memory::Allocator& allocator, usize lenght) const{
		RAINDROP_profile_function();
		RAINDROP_assert(_openMode & OPEN_READ);
		String str(allocator);
		lenght++;
		
		str.resize(lenght);

		char* out = fgets(str.data(), lenght, (FILE*)_file);

		if (out == nullptr){
			RAINDROP_log(WARNING, IO, "failed to read a part of a the file : %s", strerror(errno));
			return str;
		}

		return str;
	}

	RAINDROP_API bool Filestream::read(char* ptr, usize lenght){
		RAINDROP_profile_function();
		RAINDROP_assert(ptr);

		char* out = fgets(ptr, lenght, (FILE*)_file);


		if (out == nullptr){
			RAINDROP_log(WARNING, IO, "failed to read a part of the file : %s", strerror(errno));
			return false;
		}

		return true;
	}

	RAINDROP_API bool Filestream::read(String& string, usize lenght){
		RAINDROP_profile_function();
		if (lenght == (usize)-1){
			lenght = string.size();
		}

		fread(string.data(), lenght, 1, (FILE*)_file);
		
		// if (out == nullptr){
		// 	RAINDROP_log(WARNING, IO, "failed to read a part a file : %s", strerror(errno));
		// 	return false;
		// }

		return true;
	}
	
	RAINDROP_API void Filestream::writeChar(unsigned char c){
		RAINDROP_profile_function();
		RAINDROP_assert(_openMode & OPEN_WRITE);
		int out = fputc(c, (FILE*)_file);

		if (out == EOF){
			RAINDROP_log(ERROR, IO, "cannot write further, hit the end of the file : %s", strerror(errno));
		}
	}

	RAINDROP_API void Filestream::writeString(const String &str){
		RAINDROP_profile_function();
		writeString(str.str());
	}
	
	RAINDROP_API void Filestream::writeString(const char *str){
		RAINDROP_profile_function();
		RAINDROP_assert(_openMode & OPEN_WRITE);
		int out = fputs(str, (FILE*)_file);

		if (out == EOF){
			RAINDROP_log(ERROR, IO, "cannot write further, hit the end of the file : %s", strerror(errno));
		}
	}

	RAINDROP_API void Filestream::write(int64 i){
		RAINDROP_profile_function();
		char buf[255];
		_i64toa(i, buf, 10);
		writeString(buf);
	}

	RAINDROP_API void Filestream::write(uint64 ui){
		RAINDROP_profile_function();
		char buf[255];
		_ui64toa(ui, buf, 10);
		writeString(buf);
	}

	RAINDROP_API void Filestream::write(float64 f){
		RAINDROP_profile_function();
		char buf[255];
		gcvt(f, 9, buf);
		writeString(buf);
	}

	RAINDROP_API void Filestream::write(const char* str){
		RAINDROP_profile_function();
		writeString(str);
	}

	RAINDROP_API void Filestream::write(const char c){
		RAINDROP_profile_function();
		char buf[2] = {c, '\0'};
		writeString(buf);
	}

	RAINDROP_API uint32 Filestream::getPos() const{
		RAINDROP_profile_function();
		int pos = ftell((FILE*)_file);

		if (pos == -1L){
			RAINDROP_log(ERROR, IO, "failed to get a valid cusor position : %s", strerror(errno));
			return -1;
		}

		return (uint32)pos;
	}

	RAINDROP_API int filePosToSeek(Filestream::FilePos origin){
		RAINDROP_profile_function();
		switch (origin){
			case Filestream::POS_BEGIN: return SEEK_SET;
			case Filestream::POS_CURRENT: return SEEK_CUR;
			case Filestream::POS_END: return SEEK_END;
		}
		RAINDROP_log(WARNING, OTHER, "unknown file pos (id : %d) set as current position", (int)origin);
		return SEEK_SET;
	}

	RAINDROP_API void Filestream::setPos(int32 pos, FilePos origin){
		RAINDROP_profile_function();
		int out = fseek((FILE*)_file, (long)pos, filePosToSeek(origin));

		if (out < 0){
			RAINDROP_log(ERROR, IO, "cannot set the cursor to this position : %s", strerror(errno));
		}
	}

	RAINDROP_API void Filestream::jump(int32 offset){
		RAINDROP_profile_function();
		setPos(offset, POS_CURRENT);
	}

	RAINDROP_API bool Filestream::isOpen() const{
		RAINDROP_profile_function();
		return _file != nullptr;
	}

	RAINDROP_API Filestream::OpenModeFlag Filestream::getOpenMode() const{
		RAINDROP_profile_function();
		return _openMode;
	}
}