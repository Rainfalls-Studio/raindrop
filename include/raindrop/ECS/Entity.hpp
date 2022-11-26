/**
 * @brief this file contain the ECS entity abstraction layer
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @attention IF YOU DO ANY CHANGE IN THIS FILE, MAKE SURE THAT THE WHOLE PROJECT IS RECOMPILED
 * @authors @Aalexdev (aaleex3984@gmail.com), ... (add here)
 */

/**
 * MIT License
 * Copyright (c) 2022 Rainfalls studio
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#ifndef __RAINDROP_ECS_ENTITY_HPP__
#define __RAINDROP_ECS_ENTITY_HPP__

#include "core.hpp"
#include "ECS.hpp"

namespace rnd{
	class Entity{
		public:
			Entity(ECS::Entity handle, ECS::Registry *registry) : handle{handle}, registry{registry}{}

			Entity &operator=(ECS::Entity handle){
				PROFILE_FUNCTION();
				this->handle = handle;
				return *this;
			}

			template<typename T>
			bool has(){
				PROFILE_FUNCTION();
				return registry->has<T>(handle);
			}

			template<typename T>
			void add(T t={}){
				PROFILE_FUNCTION();
				registry->addComponent<T>(handle, t);
			}

			template<typename T>
			void remove(){
				PROFILE_FUNCTION();
				registry->removeComponent<T>(handle);
			}

			template<typename T>
			T& get(){
				PROFILE_FUNCTION();
				return registry->getComponent<T>(handle);
			}

			void destroy(){
				PROFILE_FUNCTION();
				registry->destroy(handle);
			}

		private:
			ECS::Entity handle = -1;
			ECS::Registry *registry;
	};
}

#endif