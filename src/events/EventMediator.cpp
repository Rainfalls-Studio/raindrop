// #include "events/EventMediator.hpp"
// #include "debug/Instrumentor.hpp"
// #include "debug/Logger.hpp"

// namespace rnd::events{
// 	void Mediator::init(uint32_t eventTypes, uint32_t frameCount){
// 		PROFILE_FUNCTION();
// 		this->frameCount = frameCount;

// 		events.init<EventType>(eventTypes);
// 		subscriberPool.init<memory::ForwardLinkedList>(1500, 100);
// 		callsAllocator.init<EventCall>(2500, 150);

// 		calls = (memory::ForwardLinkedList*)memory::malloc(sizeof(memory::ForwardLinkedList) * frameCount);
// 		for (int i=0; i<frameCount; i++){
// 			calls[i].init<EventCall>(&callsAllocator);
// 		}
// 	}

// 	void Mediator::shutdown(){
// 		PROFILE_FUNCTION();
// 		events.shutdown();
// 		subscriberPool.shutdown();
// 		callsAllocator.shutdown();

// 		for (int i=0; i<frameCount; i++){
// 			calls[i].shutdown();
// 		}

// 		memory::free(calls);
// 	}

// 	void Mediator::update(){
// 		PROFILE_FUNCTION();

// 	}

// 	EventID Mediator::registerEvent(const char* name, uint32_t dataSize){
// 		PROFILE_FUNCTION();

// 		EventID id = getEventFromName(name);
// 		if (id != EVENT_NONE){
// 			RND_WARN("register event", "the name is already used by another event type. Use a different name");

// 			if (get(id).dataSize != dataSize){
// 				RND_ERR("register event", "the event that use the same name, have a different data size. Use a different name or check the data size");
// 				return EVENT_NONE;
// 			}

// 			return id;
// 		}

// 		id = nextEvent;
// 		nextEvent++;

// 		if (events.size() - id <= 0){
// 			events.reserve(100);
// 		}

// 		EventType& event = events.get<EventType>(id);
// 		nameToIdMap[name] = id;
// 		event.dataSize = dataSize;
// 		event.subscribers.init<EventCall>(&callsAllocator);
		
// 		return id;
// 	}

// 	EventID Mediator::getEventFromName(const char* name){
// 		PROFILE_FUNCTION();
// 		auto it = nameToIdMap.find(name);
// 		if (it == nameToIdMap.end()){
// 			return EVENT_NONE;
// 		}
// 		return it->second;
// 	}

// 	Mediator::EventType& Mediator::get(EventID id){
// 		return events.get<EventType>(id);
// 	}
// }