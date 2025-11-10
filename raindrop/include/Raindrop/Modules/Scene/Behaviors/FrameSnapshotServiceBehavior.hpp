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
                
                private:
                    Scene::Scene* _scene;
                    Scene::BehaviorID _serviceID;
            };


            class ReleaseWrite : public Scene::IBehavior{
                public:
                    virtual ~ReleaseWrite() override = default;

                    virtual void initialize(Engine& engine, Scene::Scene& scene) override;
                    virtual void execute() override;
                
                private:
                    Scene::Scene* _scene;
                    Scene::BehaviorID _serviceID;
            };


            class LockRead : public Scene::IBehavior{
                public:
                    virtual ~LockRead() override = default;

                    virtual void initialize(Engine& engine, Scene::Scene& scene) override;
                    virtual void execute() override;
                
                private:
                    Scene::Scene* _scene;
                    Scene::BehaviorID _serviceID;
            };


            class ReleaseRead : public Scene::IBehavior{
                public:
                    virtual ~ReleaseRead() override = default;

                    virtual void initialize(Engine& engine, Scene::Scene& scene) override;
                    virtual void execute() override;
                
                private:
                    Scene::Scene* _scene;
                    Scene::BehaviorID _serviceID;
            };



            using SlotID = uint32_t;
            static constexpr SlotID INVALID_SLOT_ID = ~static_cast<SlotID>(0);

            /**
             * @brief Construct a new Frame Snapshot Service object
             * 
             * @param bufferCount default 3 to always have a free buffer at disposal
             */
            FrameSnapshotService(uint32_t bufferCount = 3);
            virtual ~FrameSnapshotService() override = default;

            virtual void initialize(Engine& engine, Scene::Scene& scene) override;


        
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

            template<typename T>
            inline void writeSlot(SlotID slot, const T& v){
                writeSlotRaw(slot, static_cast<const void*>(&v));
            }

            /**
             * @brief Write an instance data to a given slot
             * 
             * @param slot the slot to write the data into
             * @param instance the payload
             */
            void writeSlotRaw(SlotID slot, const void* instance);

            /**
             * @brief Store an undefined type of data in the arena, returning it's offset in the buffer
             * 
             * @param data the data to store
             * @param size the size of the payload
             * @return Offset 
             */
            Offset storeRaw(const void* data, size_t size);


            // --- Reading ------------------------------------------------------------------


            void lockRead();
            void readRelease();

            std::vector<Offset> readSlotOffsets(SlotID slot) const;

            template<typename T>
            inline const T& read(Offset off) const{
                return *reinterpret_cast<const T*>(readRaw(off));
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
