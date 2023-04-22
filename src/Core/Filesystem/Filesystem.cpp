#include <Core/Filesystem/Filesystem.hpp>
#include <Core/Debug/profiler.hpp>
#include <filesystem>

#ifdef RAINDROP_WINDOWS
	#include <windows.h>
	#include <tchar.h> 
	#include <strsafe.h>
	#include <io.h>
	#define F_OK 0
	#define access _access
#else
	#include <unistd.h>
#endif

#include <stdio.h>
#include <dirent.h>

#if defined(RAINDROP_LINUX) || defined(RAINDROP_POSIX)
	#include <sys/stat.h>
#endif

#ifdef ERROR
	#define D_ERROR ERROR
	#undef ERROR
#endif

#ifdef sprintf
	#undef sprintf
#endif

namespace Raindrop::Core::Filesystem{
	RAINDROP_API bool exist(const Path& path){
		RAINDROP_profile_function();
		return access(path.str(), F_OK) == 0;
	}

	RAINDROP_API Extention getExtention(const Path& path, Memory::Allocator& allocator){
		RAINDROP_profile_function();
		return (Extention)String{allocator, ((std::filesystem::path)path.str()).extension().string().c_str()};
	}

	RAINDROP_API usize getSize(const Path& path){
		RAINDROP_profile_function();
		#if defined(RAINDROP_WINDOWS)
			// with the windows API

			HANDLE hFile = CreateFile(reinterpret_cast<LPCSTR>(path.str()), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile==INVALID_HANDLE_VALUE){
				DWORD err = ::GetLastError();
				RAINDROP_log(ERROR, IO, "failed to find the file : \"%s\"", path.str());
				return -1;
			}

			LARGE_INTEGER size;
			if (!::GetFileSizeEx(hFile, &size)){
				RAINDROP_log(ERROR, IO, "failed to get file size, the file maybe does not exist or is protected : \"%s\"", path.str());
				return -1;
			}

			CloseHandle(hFile);
			
			return (usize)size.QuadPart;

		#elif defined(RAINDROP_LINUX) || defined(RAINDROP_POSIX)
			// with the linux / POSIX API
			struct stat st;
			stat(filename, &st);
			return (usize)st.st_size;

		#else
			// standart

			FILE* f = fopen(path.str(), "r");
			if (f == nullptr){
				RAINDROP_log(ERROR, IO, "failed to open file : \"%s\"", path.str());
				return -1;
			}

			fseek(f, 0, SEEK_END);
			usize size = ftell(f);

			fclose(f);
			return size;
		#endif
	}

	RAINDROP_API String getName(const Path& path, Memory::Allocator& allocator){
		RAINDROP_profile_function();
		return String{allocator, std::filesystem::path(path.str()).filename().string().c_str()};
	}

	RAINDROP_API Memory::Array<String> getFolderContent(Memory::Allocator& allocator, const Path& path){		
		RAINDROP_profile_function();
		#ifdef RAINDROP_WINDOWS
			TCHAR szDir[MAX_PATH];
			StringCchCopy(szDir, MAX_PATH, reinterpret_cast<STRSAFE_LPCSTR>(path.str()));
			StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
			
			HANDLE hFind = INVALID_HANDLE_VALUE;
			WIN32_FIND_DATA ffd;

			hFind = FindFirstFile(szDir, &ffd);

			if (hFind == INVALID_HANDLE_VALUE){
				RAINDROP_log(ERROR, IO, "failed to find the folder : \"%s\"", path.str());
				return Memory::Array<String>(allocator, 0);
			}

			usize contentSize = 0;
			while (true){
				contentSize++;
				if (FindNextFile(szDir, &ffd) == FALSE) break; 
			}

			Memory::Array<String> content(allocator, contentSize);
			FindFirstFile(szDir, &ffd);

			usize i = 0;
			while (true){
				content[i] = String(allocator, (char*)ffd.cFileName);
				i++;
				if (FindNextFile(szDir, &ffd) == FALSE) break; 
			}

			return content;
		#else
			DIR *d = nullptr;
			struct dirent *dir;
			d = opendir(".");
			
			if (!d){
				RAINDROP_log(ERROR, IO, "failed to open folder : \"%s\"", path.str());
				return Memory::Array<String>(allocator, 0);
			}

			usize contentSize = 0;
			DIR *counterDir = d;

			while ((dir = readdir(counterDir)) != nullptr){
				contentSize++;
			}

			Memory::Array<String> content(allocator, contentSize);

			usize i = 0;
			while ((dir = readdir(d)) != nullptr){
				content[i] = dir->d_name;
				i++;
			}

			return content;
		#endif
	}

	RAINDROP_API bool copy(const Path& src, const Path& dst){
		RAINDROP_profile_function();
		try{
			std::filesystem::copy(src.str(), dst.str());
		} catch (std::exception &e){
			RAINDROP_log(ERROR, IO, "failed to copy a file or a folder : \"%s\"", e.what());
			return false;
		}
		return true;
	}

	RAINDROP_API bool move(const Path& src, const Path& dst){
		RAINDROP_profile_function();
		#ifdef RAINDROP_WINDOWS
			return (bool)MoveFileA((LPCSTR)src.str(), (LPCSTR)dst.str());
		#else
			return ::rename(src.str(), dst.str()) == 0;
		#endif
	}

	RAINDROP_API bool remove(const Path& path){
		RAINDROP_profile_function();
		#ifdef RAINDROP_WINDOWS
			return (bool)DeleteFileA((LPCSTR)path.str());
		#else
			return ::remove(path.str()) == 0;
		#endif
	}
}