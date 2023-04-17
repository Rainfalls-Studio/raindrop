#ifndef __RAINDROP_CORE_STRING_HPP__
#define __RAINDROP_CORE_STRING_HPP__

#include <string>
#include <common.hpp>
#include <Core/Memory/allocators/Allocator.hpp>
#include "Tools/hash.hpp"

namespace Raindrop::Core{
	class String{
		public:
			/**
			 * @brief create a string of character from 'chr'
			 * @param allocator the allocator to allocate memory from
			 * @param chr the string of characters to copy (copy only the content)
			 */
			String(Memory::Allocator& allocator, const char* chr);

			/**
			 * @brief copy the 'other' string and create a new one
			 * @param allocator the allocator to allocate memory from
			 * @param other the string to copy (copy only the content)
			 */
			String(Memory::Allocator& allocator, const String &other);

			/**
			 * @brief copy the 'other' string and create a new one
			 * @param allocator the allocator to allocate memory from
			 * @param other the string to copy (copy only the content)
			 */
			String(Memory::Allocator& allocator, String &&other);

			/**
			 * @brief create a string without any content
			 * @param allocator the allocator to allocate memory from
			 */
			String(Memory::Allocator& allocator);

			/**
			 * @brief create a string of character from 'arg...'
			 * @param allocator the allocator to allocate memory from
			 * @param args the args to copy into the string
			 */
			template<typename... Args>
			String(Memory::Allocator& allocator, Args&... args){
				appendT<Args...>(args...);
			}

			/**
			 * @brief copy 'other'
			 * @param other the string to copy (copy the also allocator)
			 */
			String& operator=(const String &other);

			/**
			 * @brief return the lenght of the string
			 * @return usize 
			 */
			usize size() const;

			/**
			 * @brief check if the string is empty
			 * @return true if empty, false otherwise
			 */
			bool empty() const;

			/**
			 * @brief return the content of the string as a const array of c style string
			 * @return const char* 
			 */
			const char* str() const;

			/**
			 * @brief return the content of the string as a modifyable array of c style string
			 * @return const char* 
			 */
			char* data();

			/**
			 * @brief resize the array
			 * @param size the new size of the array. 
			 */
			void resize(usize size);

			/**
			 * @brief clear the string and resize it as 0
			 * 
			 */
			void clear();

			/**
			 * @brief push the character 'c' into the string
			 * @param c the character to push
			 */
			void append(const char c);

			/**
			 * @brief push the string 'str' into the string
			 * @param str the string to push
			 */
			void append(const char *str);

			/**
			 * @brief push the string 'other' into the string
			 * @param other the string to push
			 */
			void append(const String &other);

			/**
			 * @brief get the character at the 'i' index
			 * @param i the index to read the character from
			 * @return const char& 
			 */
			const char &operator[](usize i) const;
			
			/**
			 * @brief get the character at the 'i' index
			 * @param i the index of the character to read
			 * @return char& 
			 */
			char &operator[](usize i);

			/**
			 * @brief append 't' into the string
			 * 
			 * @tparam T the type to append
			 * @param t the element to append
			 * @return String& 
			 */
			template<typename T>
			String &operator+=(const T &t){
				append(t);
				return *this;
			}

			/**
			 * @brief check if the string is equal to 'str'
			 * @param str the string to compare to
			 * @return true if equal, false otherwise
			 */
			bool operator==(const char *str) const;

			/**
			 * @brief check if the string is equal to 'other'
			 * @param other the string to compare to
			 * @return true if equal, false otherwise
			 */
			bool operator==(const String &other) const;

			ID64 hash64() const;

		private:
			std::string _string;

			template<typename T, typename A, typename... Args>
			void appendT(T& t, A& a, Args&... args){
				append(t);
				appendT(a, args...);
			}

			template<typename T>
			void appendT(T& t){
				append(t);
			}
	};
}

#endif