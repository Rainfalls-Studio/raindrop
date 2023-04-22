#ifndef __RAINDROP_CORE_FILSYSTEM_FILESTREAM_HPP__
#define __RAINDROP_CORE_FILSYSTEM_FILESTREAM_HPP__

#include <Core/Filesystem/Filesystem.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include <Core/Memory/containers/Array.hpp>

namespace Raindrop::Core::Filesystem{
	class File;
	class RAINDROP_API Filestream{
		public:
			enum OpenMode{
				OPEN_READ   = 1 << 0,
				OPEN_WRITE  = 1 << 1,
				OPEN_APPEND = 1 << 2,
				OPEN_BEGIN  = 1 << 3,
				OPEN_BINARY = 1 << 4,
			};

			enum FilePos{
				POS_BEGIN,
				POS_CURRENT,
				POS_END,
			};

			using OpenModeFlag = uint8;

			Filestream(const File& file, OpenModeFlag openMode);
			Filestream(const Path& path, OpenModeFlag openMode);
			Filestream();
			~Filestream();

			/**
			 * @brief open a file from a path
			 * 
			 * @param path the path to the file to open
			 * @param openMode the open mode
			 * @return true on success
			 */
			bool open(const Path& path, OpenModeFlag openMode);

			/**
			 * @brief open a file from a path
			 * 
			 * @param path the path to the file to open
			 * @param openMode the open mode
			 * @return true on success
			 */
			bool open(const char* path, OpenModeFlag openMode);

			/**
			 * @brief open a file
			 * 
			 * @param file the file to open
			 * @param openMode the open mode
			 * @return true on success
			 */
			bool open(const File& file, OpenModeFlag openMode);

			/**
			 * @brief close the file if opened, those nothing if not open
			 * 
			 */
			void close();

			/**
			 * @brief get if the file is opened
			 * @return true 
			 * @return false 
			 */
			bool isOpen() const;

			/**
			 * @brief get the opening mode
			 * @return OpenModeFlag 
			 */
			OpenModeFlag getOpenMode() const;

			/**
			 * @brief apply the modifications and write the file on the disk
			 * 
			 */
			void flush();

			/**
			 * @brief get the next character of the file and go forward by one byte
			 * @return unsigned char 
			 */
			unsigned char readChar(bool *end = nullptr) const;

			/**
			 * @brief get the next 'lenght' characters backed into a string
			 * @note the lenght of the string will be 'lenght' + 1, a '\0' character is added at the end of the string
			 * 
			 * @param allocator the allocator to allocater the string
			 * @param lenght the number of bytes to read
			 * @return String 
			 */
			String readString(Memory::Allocator& allocator, usize lenght) const;

			/**
			 * @brief copy the next 'lenght' characters into the string
			 * 
			 * @param ptr the dst buffer
			 * @param size the count of characters to read
			 */
			bool read(char* ptr, usize lenght);

			/**
			 * @brief copy the next 'lenght' character into the array, is 'lenght' is not override, it will be set as array.size();
			 * 
			 * @param array the dst 
			 * @param lenght 
			 */
			bool read(String& string, usize lenght = (-1));

			/**
			 * @brief write a single character to the file
			 * @note call flush to update the file
			 * 
			 * @param c the character to write
			 */
			void writeChar(unsigned char c);

			/**
			 * @brief write a string to the file
			 * @note call flush to update the file
			 * 
			 * @param str the string to write
			 */
			void writeString(const String &str);
			
			/**
			 * @brief write a string to the file
			 * @note call flush to update the file
			 * 
			 * @param str the string to write
			 */
			void writeString(const char *str);

			void write(int64 i);
			void write(uint64 ui);
			void write(float64 f);
			void write(const char* str);
			void write(const char c);

			template<typename... Args>
			void write(Args... args){
				writeT<Args...>(args...);
			}

			/**
			 * @brief get the position from the begining of the file (in bytes)
			 * @return uint32 
			 */
			uint32 getPos() const;

			/**
			 * @brief set the cursor's position relative to the origin
			 * @param pos the position (in bytes)
			 * @param origin the origin of the pos
			 */
			void setPos(int32 pos, FilePos origin = POS_BEGIN);

			/**
			 * @brief jump the specified amount of bytes
			 * @param offset the number of bytes to jump from the current pos
			 */
			void jump(int32 offset);

		private:
			OpenModeFlag _openMode;
			void* _file;

			template<typename T, typename U, typename... Args>
			void writeT(T& t, U& u, Args&... args){
				write(t);
				writeT(u, args...);
			}

			template<typename T>
			void writeT(T& t){
				write(t);
			}
	};
}

#endif