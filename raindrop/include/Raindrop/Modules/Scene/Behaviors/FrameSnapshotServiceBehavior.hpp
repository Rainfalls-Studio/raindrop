#pragma once

#include "../IBehavior.hpp"

namespace Raindrop::Behaviors{
    
    using Offset = uint32_t;
    
    class FrameSnapshotService : public Scene::IBehavior{
        public:
            class LockWrite : public Scene::IBehavior{
                public:
                    virtual ~LockWrite() override = default;

                    virtual void initialize(Engine& engine, Scene::Scene& scene) override;
                    virtual void execute() override;

                    virtual const char* name() const override;
                
                private:
                    Scene::Scene* _scene;
                    Scene::BehaviorID _serviceID;
            };


            class ReleaseWrite : public Scene::IBehavior{
                public:
                    virtual ~ReleaseWrite() override = default;

                    virtual void initialize(Engine& engine, Scene::Scene& scene) override;
                    virtual void execute() override;

                    virtual const char* name() const override;
                
                private:
                    Scene::Scene* _scene;
                    Scene::BehaviorID _serviceID;
            };


            class LockRead : public Scene::IBehavior{
                public:
                    virtual ~LockRead() override = default;

                    virtual void initialize(Engine& engine, Scene::Scene& scene) override;
                    virtual void execute() override;

                    virtual const char* name() const override;
                
                private:
                    Scene::Scene* _scene;
                    Scene::BehaviorID _serviceID;
            };


            class ReleaseRead : public Scene::IBehavior{
                public:
                    virtual ~ReleaseRead() override = default;

                    virtual void initialize(Engine& engine, Scene::Scene& scene) override;
                    virtual void execute() override;

                    virtual const char* name() const override;
                
                private:
                    Scene::Scene* _scene;
                    Scene::BehaviorID _serviceID;
            };

            using SlotID = uint32_t;
            static constexpr SlotID INVALID_SLOT_ID = ~static_cast<SlotID>(0);

            template<typename T>
            struct Handle{
                Handle(Offset offset, FrameSnapshotService& service) : offset{offset}, service{service}{}
                Handle(Handle<T>&& ) = default;
                Handle(const Handle<T>&) = default;

                template<typename U> requires std::is_convertible_v<U*, T*> || std::is_void_v<U>
                Handle(const Handle<U>& other) : offset(other.offset), service(other.service) {}

                Offset offset;
                FrameSnapshotService& service;
                
                inline T* get(){
                    return static_cast<T*>(service.resolveWrite(offset));
                }

                inline T* operator->(){
                    return get();
                }

                inline Handle<T> operator[](size_t i){
                    return Handle<T>(offset + sizeof(T) * i, service);
                }

                inline operator Offset() const{
                    return offset;
                }
            };

            /**
             * @brief Construct a new Frame Snapshot Service object
             * 
             * @param bufferCount default 3 to always have a free buffer at disposal
             */
            FrameSnapshotService(uint32_t bufferCount = 3);
            virtual ~FrameSnapshotService() override = default;

            virtual void initialize(Engine& engine, Scene::Scene& scene) override;

            virtual const char* name() const override;


        
            // --- Slots ------------------------------------------------------------------


            template<typename T>
            inline SlotID registerSlot(const std::string& name = typeid(T).name()){
                return registerSlot(typeid(T), static_cast<uint32_t>(sizeof(T)), name);
            }

            /**
             * @brief Creates a new data slot
             * 
             * @param size the size of an instance in the slot
             * @param name the debug name of the slot
             * @return SlotID it's unique ID
             */
            SlotID registerSlot(const std::type_index& type, uint32_t size, const std::string& name);
            
            /**
             * @brief Get the slot ID linked to the type
             * 
             * @tparam T the type
             * @return the ID or INVALID_SLOT_ID if not registred
             */
            template<typename T>
            inline SlotID getSlotID() const{
                return getSlotID(typeid(T));
            }

            /**
             * @brief Get the slot ID linked to the type
             * 
             * @param type 
             * @return the ID or INVALID_SLOT_ID if not registred
             */
            SlotID getSlotID(const std::type_index& type) const;


            // --- Writing ------------------------------------------------------------------


            void lockWrite();
            void writeRelease();

            /**
             * @brief Allocate a buffer of size 'size' in the current buffer arena
             * @warning doing this may invalidate all pointers to the arena (as it may reallocate). This is why we use handles
             * 
             * @param size the size of the buffer
             * @return Offset 
             */
            Offset allocate(size_t size);

            /**
             * @brief Used to write an instance data to a given slot
             * 
             * @tparam T the type of the data
             * @param slot the slot to write to
             * @return Handle<T> a handle to the allocated slot
             */
            template<typename T>
            inline Handle<T> writeSlot(SlotID slot){
                return Handle<T>(writeSlotRaw(slot));
            }

            /**
             * @brief Used to write an instance data to a given slot
             * 
             * @param slot the slot to write to
             * @return Handle<void*> a handle to the allocated slot
             */
            Handle<void> writeSlotRaw(SlotID slot);


            /**
             * @brief Used to write an undefined ammount of data to the current write buffer
             * 
             * @return Handle<void*> a handle to the allocated buffer
             */
            Handle<void> writeRaw(size_t size);

            /**
             * @brief Used to write data to the current write buffer's arena
             * 
             * @tparam T the type of the data
             * @param n The number of instances, usually 1 but can be greater in case of arrays
             * @return Handle<T> a handle to the allocated buffer
             */
            template<typename T>
            inline Handle<T> write(size_t n=1){
                return Handle<T>(writeRaw(sizeof(T) * n));
            }

            /**
             * @brief Return the pointer of the given offset in the current write buffer arena. This pointer is valid until an allocation operation is made
             * 
             * @param offset the offset in the arena
             * @return void* 
             */
            void* resolveWrite(Offset offset);


            // --- Reading ------------------------------------------------------------------


            void lockRead();
            void readRelease();

            const std::vector<Offset>& readSlotOffsets(SlotID slot) const;

            template<typename T>
            inline const T& read(Offset off) const{
                return *reinterpret_cast<const T*>(readRaw(off));
            }

            template<typename T>
            inline const T* readPtr(Offset off) const{
                return reinterpret_cast<const T*>(readRaw(off));
            }

            const void* readRaw(Offset off) const;
        
        private:
            bool hasReadyUnsafe() const;

            static constexpr uint32_t INVALID_BUFFER_INDEX = std::numeric_limits<uint32_t>::max();

            // Scene::Scene* _scene;
            enum class State : uint8_t{
                FREE,
                WRITING,
                READY,
                READING
            };

            struct Slot{
                std::string name;
                uint32_t size;
            };

            struct SlotData{
                SlotID id;
                uint32_t size; // cache of Slot::size for cache friendliness
                std::vector<Offset> instances;
            };

            struct Buffer{
                std::vector<std::byte> arena;
                std::vector<SlotData> slots;
                State state;
            };

            uint32_t _bufferCount;

            uint32_t _writeIndex; // the buffer that is currently being written onto
            uint32_t _readIndex; // the buffer that is currently being read
            uint32_t _latestWrite; // the most recently finished write buffer


            mutable std::mutex _stateMutex;
            std::condition_variable _cvCanWrite;
            std::condition_variable _cvCanRead;
            
            std::vector<Buffer> _buffers;
            std::vector<Slot> _slots;
            std::unordered_map<std::type_index, SlotID> _typeToSlot;
    };
}
