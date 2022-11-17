
/**
 * @brief thes file contain the event mediator. This class handle event calls over the engine
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
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
 * 
 */

#ifndef __RAINDROP_EVENTS_EVENT_MEDIATOR_HPP__
#define __RAINDROP_EVENTS_EVENT_MEDIATOR_HPP__

#include "core.hpp"
#include "debug/Instrumentor.hpp"
#include "memory/allocators/Moya.hpp"

namespace rnd::events{
	class Mediator{
		public:
			// if the functions/methods return true, that mean that the event ha sbeen handled and should no longer be send to other callbacks
			using EventFn = bool(*)(void*); // event function (the data pointer)
			using EventMt = bool(*)(void*, void*); // event method (the instance pointer and the data pointer)

			void init(uint32_t frameCount = 2){
				PROFILE_FUNCTION();
				this->frameCount = frameCount;

				createDataBuffers();
				createCalls();
			}

			void shutdown(){
				PROFILE_FUNCTION();
				destroyCalls();
				destroyDataBuffers();
			}

			EventID registerEvent(const char* name, uint32_t size = 0){
				PROFILE_FUNCTION();

				EventID id = EVENT_NONE;
				{
					id = getEventID(name);
					if (id != EVENT_NONE){
						Event &e = events[id];
						RND_ASSERT(e.dataSize == size, "the event name is already used, but the data size is different");
						return id;
					}
				}

				id = events.size();
				nameToIDMap[name] = id;

				const std::string& str = nameToIDMap.find(name)->first;
				Event event(str);
				event.dataSize = size;

				events.push_back(event);
				return id;
			}

			template<typename... Args>
			EventID registerEvent(const char* name){
				PROFILE_FUNCTION();
				return registerEvent(name, sizeof...(Args));
			}

			EventID getEventID(const char* name){
				PROFILE_FUNCTION();
				auto it = nameToIDMap.find(name);
				if (it == nameToIDMap.end()) return EVENT_NONE;
				return it->second;
			}

			bool exist(const char* name){
				PROFILE_FUNCTION();
				return nameToIDMap.find(name) != nameToIDMap.end();
			}

			void subscribe(EventID id, EventMt mt, void* instance){
				PROFILE_FUNCTION();

				Event& event = get(id);
				Subscriber sub;

				if (instance){
					sub.instance = instance;
					sub.methode = mt;
				} else {
					sub.instance = nullptr;
					sub.funtion = (EventFn)mt;
				}

				event.subscribers.push_back(sub);
			}

			void subscribe(EventID id, EventFn fn){
				PROFILE_FUNCTION();
				subscribe(id, (EventMt)fn, nullptr);
			}

			template<typename... Args>
			void trigger(EventID id, const Args&... args){
				PROFILE_FUNCTION();

				uint32_t size = get(id).dataSize;
				RND_ASSERT(size == sizeof...(Args), "the args given are different that what the event require. Check the args and make sure you are triggering the right event");

				void* ptr = allocateData(size);
				copy((char*)ptr, args...);

				Call call;
				call.event = id;
				call.data = ptr;

				calls->push_back(call);
			}

			template<typename... Args>
			void trigger(const char* name, const Args&... args){
				PROFILE_FUNCTION();
				trigger(getEventID(name), args...);
			}

			void update(){
				PROFILE_FUNCTION();
				auto &list = calls[currentFrame];
				for (auto &c : list){
					Event& event = get(c.event);
					triggerEvent(event, c.data);
				}

				list.clear();
				dataBuffers[currentFrame].reset();
				currentFrame = (currentFrame + 1) % frameCount;
			}

			void unsubscribe(EventID id, EventMt mt, void* instance){
				PROFILE_FUNCTION();

				Event& event = get(id);

				auto it = event.subscribers.begin();

				while (it != event.subscribers.end()){
					bool found = false;

					if (instance){
						found = it->instance == instance && it->methode == mt;
					} else {
						found = it->funtion == (EventFn)mt;
					}

					if (found){
						event.subscribers.erase(it);
						break; // the subscriber is supposedly only once in the subscriber list
					}

					it++;
				}
			}

			void unsubscribe(EventID id, EventFn fn){
				PROFILE_FUNCTION();
				unsubscribe(id, (EventMt)fn, nullptr);
			}		

		private:
			void createCalls(){
				PROFILE_FUNCTION();
				destroyCalls();
				calls = new std::list<Call, CallAllocator>[frameCount];
			}

			void createDataBuffers(){
				PROFILE_FUNCTION();
				destroyDataBuffers();
				dataBuffers = new Stack<>[frameCount];

				for (int i=0; i>frameCount; i++){
					dataBuffers[i].init(50000); // TODO : RAI-19
				}
			}

			void destroyCalls(){
				PROFILE_FUNCTION();
				if (calls){
					delete[] calls;
				}
				calls = nullptr;
			}

			void destroyDataBuffers(){
				PROFILE_FUNCTION();
				if (dataBuffers){
					delete[] dataBuffers;
				}
				dataBuffers = nullptr;
			}

			struct Subscriber{
				union{
					EventFn funtion;
					EventMt methode;
				};

				// if the subscriber has an instance, it's a methode, if not, it's a function
				void* instance = nullptr;
			};
			
			struct Call{
				EventID event;
				void* data;
			};

			using CallAllocator = Moya::Allocator<Call>;

			struct Event{
				Event(const std::string& name) : name{name}{}
				List<Subscriber> subscribers{};
				uint32_t dataSize = 0;
				const std::string& name;
			};

			template<typename T, typename U, typename... Args>
			void copy(char* dst, T& t, U& u, Args&... args){
				PROFILE_FUNCTION();
				memory::memcpy(dst, &t, sizeof(T));
				dst += sizeof(T);
				copy(u, args...);
			}

			template<typename T>
			void copy(char* dst, T& t){
				PROFILE_FUNCTION();
				memory::memcpy(dst, &t, sizeof(T));
			}

			void copy(char* dst){}

			void triggerEvent(Event &event, void* data){
				PROFILE_FUNCTION();
				for (auto &s : event.subscribers){
					if (s.instance){
						if (s.methode(s.instance, data)) break;
					} else {
						if (s.funtion(data)) break;
					}
				}
			}

			uint32_t frameCount = 0;
			Map<std::string, EventID> nameToIDMap{};
			DynamicArray<Event> events;
			
			List<Call, CallAllocator>* calls = nullptr;
			Stack<>* dataBuffers = nullptr;

			uint8_t currentFrame = 0;

			void* allocateData(uint32_t size){
				PROFILE_FUNCTION();
				return dataBuffers[currentFrame].allocate(size);
			}

			Event& get(EventID id){
				PROFILE_FUNCTION();
				RND_ASSERT(id < events.size(), "event id out of range");
				return events[id];
			}
	};
}

#endif
