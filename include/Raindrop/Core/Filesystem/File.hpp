#ifndef __RAINDROP_CORE_FILESYSTEM_FILE_HPP__
#define __RAINDROP_CORE_FILESYSTEM_FILE_HPP__

#include <Core/Filesystem/Filesystem.hpp>
#include <Core/Filesystem/Filestream.hpp>
#include <Core/Memory/allocators/Allocator.hpp>

namespace Raindrop::Core::Filesystem{
	class RAINDROP_API File{
		public:
			File(const File& other);
			File(const Path& path);
			File(Memory::Allocator& allocator, const char* path);
			~File();

			/**
			 * @brief give the pass given at the construction of the object
			 * @return const Path& 
			 */
			const Path &path() const;
			
			/**
			 * @brief get the size of the file if exist, if not return (usize)-1
			 * @return const usize 
			 */
			const usize size() const;

			/**
			 * @brief get the name of the file
			 * @return String 
			 */
			String name(Memory::Allocator& allocator) const;

			/**
			 * @brief get the extention of the file
			 * @return the extention as a string, an empty string if there is none
			 */
			Extention extention(Memory::Allocator& allocator) const;

			/**
			 * @brief get if the file exist
			 * @return true if exist, false otherwise
			 */
			bool exist() const;

			/**
			 * @brief copy the file to the new path
			 * 
			 * @param path the path to copy to
			 * @return true on success, false otherwise
			 */
			bool copy(const String &path);

			/**
			 * @brief move the file to a new path
			 * @note can also be used to rename the file
			 * 
			 * @param path the new path of the file
			 * @return true on success, false otherwise
			 */
			bool move(const String &path);
			
			/**
			 * @brief delete the file from the disk
			 * @return true on success, false otherwise
			 */
			bool remove();

			/**
			 * @brief open the file and return a filestream instance
			 * 
			 * @param allocator the allocator to allocate the filestream
			 * @param mode the open mode, see Filestream.hpp
			 * @return Filestream* 
			 */
			Filestream* open(Memory::Allocator& allocator, Filestream::OpenModeFlag mode);
		private:
			Path _path;
	};
}

#endif