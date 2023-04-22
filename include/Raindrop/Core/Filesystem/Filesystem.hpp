#ifndef __RAINDROP_CORE_FILESYSTEM_HPP__
#define __RAINDROP_CORE_FILESYSTEM_HPP__

#include <common.hpp>
#include <Core/String.hpp>
#include <Core/Memory/containers/Array.hpp>

namespace Raindrop::Core::Filesystem{
	using Path = String;
	using Extention = String;

	/**
	 * @brief check if there is a folder / file at the given path
	 * 
	 * @param path the path to ckeck
	 * @return true 
	 * @return false 
	 */
	RAINDROP_API bool exist(const Path& path);

	/**
	 * @brief get the extention of a file, return an empty extention of none
	 * 
	 * @param path the 
	 * @return Extention 
	 */
	RAINDROP_API Extention getExtention(const Path& path, Memory::Allocator& allocator);
	
	/**
	 * @brief get the size of a file
	 * 
	 * @param path the file to get the size from
	 * @return usize 
	 */
	RAINDROP_API usize getSize(const Path& path);
	
	/**
	 * @brief get the name of the file / folder from the path
	 * 
	 * @param path the path to the file / folder
	 * @return String 
	 */
	RAINDROP_API String getName(const Path& path, Memory::Allocator& allocator);
	
	/**
	 * @brief get the content of a folder as an array of files names
	 * 
	 * @param allocator the allocator to allocate the array and the strings
	 * @param path the path to the folder to open
	 * @return Memory::Array<String> 
	 */
	RAINDROP_API Memory::Array<String> getFolderContent(Memory::Allocator& allocator, const Path& path);
	
	/**
	 * @brief copy the 'src' file and create / overwrite 'dst' file wit 'src's content
	 * 
	 * @param src the path of the source file, the one who will be copied
	 * @param dst the destination of the copy 
	 * @return true on success, false otherwise
	 */
	RAINDROP_API bool copy(const Path& src, const Path& dst);

	/**
	 * @brief move the 'src' file to the 'dst' path, same as copy and remove
	 * 
	 * @param src the path of the source file, the one who will be moved
	 * @param dst the new path of the file 
	 * @return true on success, false otherwise
	 */
	RAINDROP_API bool move(const Path& src, const Path& dst);

	/**
	 * @brief remove 'path' file / folder from the disk
	 * 
	 * @param path the file / folder to remove
	 */
	RAINDROP_API bool remove(const Path& path);
}

#endif