#include "Raindrop/Modules/Scene/Behaviors/FrameSnapshotServiceBehavior.hpp"

namespace Raindrop::Behaviors{
    void FrameSnapshotService::LockWrite::initialize(Engine&, Scene::Scene& scene){
        _scene = &scene;
        _serviceID = scene.getBehaviorIndex<FrameSnapshotService>();
    }

    void FrameSnapshotService::LockWrite::execute(){
        if (_serviceID == Scene::INVALID_BEHAVIOR_ID) return;
        auto service = _scene->getBehavior<FrameSnapshotService>(_serviceID);

        service->lockWrite();
    }


    void FrameSnapshotService::ReleaseWrite::initialize(Engine&, Scene::Scene& scene){
        _scene = &scene;
        _serviceID = scene.getBehaviorIndex<FrameSnapshotService>();
    }

    void FrameSnapshotService::ReleaseWrite::execute(){
        if (_serviceID == Scene::INVALID_BEHAVIOR_ID) return;
        auto service = _scene->getBehavior<FrameSnapshotService>(_serviceID);

        service->writeRelease();
    }


    void FrameSnapshotService::LockRead::initialize(Engine&, Scene::Scene& scene){
        _scene = &scene;
        _serviceID = scene.getBehaviorIndex<FrameSnapshotService>();
    }

    void FrameSnapshotService::LockRead::execute(){
        if (_serviceID == Scene::INVALID_BEHAVIOR_ID) return;
        auto service = _scene->getBehavior<FrameSnapshotService>(_serviceID);

        service->lockRead();
    }


    void FrameSnapshotService::ReleaseRead::initialize(Engine&, Scene::Scene& scene){
        _scene = &scene;
        _serviceID = scene.getBehaviorIndex<FrameSnapshotService>();
    }

    void FrameSnapshotService::ReleaseRead::execute(){
        if (_serviceID == Scene::INVALID_BEHAVIOR_ID) return;
        auto service = _scene->getBehavior<FrameSnapshotService>(_serviceID);

        service->readRelease();
    }

    

    FrameSnapshotService::FrameSnapshotService(uint32_t bufferCount) : 
        _bufferCount(bufferCount),
        _writeIndex(0),
        _readIndex(INVALID_BUFFER_INDEX),
        _latestWrite(INVALID_BUFFER_INDEX)
    {
        assert(_bufferCount >= 2 && "A least double buffering");

        _buffers.assign(
            _bufferCount,
            {
                .arena = {},
                .slots = {},
                .state = State::FREE
            }
        );
    }

    void FrameSnapshotService::initialize(Engine&, Scene::Scene&){

    }

    FrameSnapshotService::SlotID FrameSnapshotService::registerSlot(const std::type_index& type,uint32_t size, const std::string& name){
        std::lock_guard lk(_stateMutex);

        if (auto it = _typeToSlot.find(type); it != _typeToSlot.end()){
            return it->second;
        }

        SlotID id = static_cast<SlotID>(_slots.size());
        _slots.emplace_back(name, size);

        for (auto& buf : _buffers){
            buf.slots.emplace_back(SlotData{id, size, {}});
        }

        _typeToSlot[type] = id;

        return id;
    }

    FrameSnapshotService::SlotID FrameSnapshotService::getSlotID(const std::type_index& type) const{
        std::lock_guard lk(_stateMutex);
        auto it = _typeToSlot.find(type);
        if (it != _typeToSlot.end()){
            return it->second;
        }
        return INVALID_SLOT_ID;
    }


    // --- Writing ------------------------------------------------------------------


    void FrameSnapshotService::lockWrite(){
        std::unique_lock lk(_stateMutex);

        const uint32_t next = (_writeIndex + 1) % _bufferCount;
        _cvCanWrite.wait(lk, [&]{
            auto state = _buffers[next].state;
            return state == State::FREE || state == State::READY;
        });

        _writeIndex = next;
        auto& buf = _buffers[_writeIndex];

        buf.state = State::WRITING;
        buf.arena.clear();

        for (auto& slot : buf.slots){
            slot.instances.clear();
        }
    }

    void FrameSnapshotService::writeRelease(){
        std::lock_guard lk(_stateMutex);
        
        auto& buf = _buffers[_writeIndex];
        assert(buf.state == State::WRITING);

        buf.state = State::READY;
        _latestWrite = _writeIndex;

        _cvCanRead.notify_one();
    }

    void FrameSnapshotService::writeSlotRaw(SlotID slot, const void* instance){
        assert(slot < _slots.size());

        auto& buf = _buffers[_writeIndex];
        auto& slotData = buf.slots[slot];

        // store data in arena and reference offset
        Offset off = storeRaw(instance, slotData.size);
        slotData.instances.push_back(off);
    }

    Offset FrameSnapshotService::storeRaw(const void* data, size_t size){
        assert(data != nullptr);
        assert(_writeIndex != INVALID_BUFFER_INDEX);

        auto& buf = _buffers[_writeIndex];
        Offset off = static_cast<Offset>(buf.arena.size());
        
        buf.arena.resize(off + size);
        std::memcpy(buf.arena.data() + off, data, size);

        return off;
    }

    // --- Reading ------------------------------------------------------------------

    bool FrameSnapshotService::hasReadyUnsafe() const{
        if (_latestWrite >= _bufferCount) return false;
        auto state = _buffers[_latestWrite].state;

        // if either it as just been writen on or has been alread read
        return state == State::READY || state == State::FREE;
    }

    void FrameSnapshotService::lockRead(){
        std::unique_lock lk(_stateMutex);
        _cvCanRead.wait(lk, [&]{return hasReadyUnsafe();});
        
        const uint32_t index = _latestWrite;
        auto& buf = _buffers[index];

        buf.state = State::READING;
        _readIndex = index;
    }

    void FrameSnapshotService::readRelease(){
        std::lock_guard<std::mutex> lk(_stateMutex);
        assert(_readIndex != INVALID_BUFFER_INDEX);

        auto& buf = _buffers[_readIndex];
        assert(_readIndex < _bufferCount && buf.state == State::READING);

        buf.state = State::FREE;
        _readIndex = INVALID_BUFFER_INDEX;

        _cvCanWrite.notify_one();
    }

    std::vector<Offset> FrameSnapshotService::readSlotOffsets(SlotID slot) const{
        assert(slot < _slots.size());
        assert(_readIndex != INVALID_BUFFER_INDEX);

        const auto& buf = _buffers[_readIndex];
        const auto& slotData = buf.slots[slot];

        return slotData.instances;
    }

    const void* FrameSnapshotService::readRaw(Offset off) const{
        assert(_readIndex != INVALID_BUFFER_INDEX);
        const auto& buf = _buffers[_readIndex];

        assert(off < buf.arena.size());
        return buf.arena.data() + off;
    }
}