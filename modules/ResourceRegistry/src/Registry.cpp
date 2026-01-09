#include "ResourceRegistry/Registry.hpp"


extern "C" RAINDROP_EXPORT Raindrop::IModule* CreateModule(){
	return new ResourceRegistry::Registry();
}

extern "C" RAINDROP_EXPORT void DestroyModule(Raindrop::IModule* module){
	delete module;
}


namespace ResourceRegistry{
    Raindrop::Result Registry::initialize(Raindrop::InitHelper&){
        return Raindrop::Result::Success();
    }

    void Registry::destroy(RRID rid) {
        Slot& s = slotChecked(rid);

        // Remove from name map first
        auto it = _nameToSlot.find(s.name);
        if (it != _nameToSlot.end()) _nameToSlot.erase(it);

        // Free wrapper
        s.wrapper.reset();
        s.wrapperType = std::type_index(typeid(void));
        s.valueType = std::type_index(typeid(void));
        s.name.clear();
        s.alive = false;

        // Invalidate all existing handles by incrementing generation
        ++_generations[rid.id];

        // Put slot index back into freelist
        _freeList.push_back(rid.id);
    }

    uint32_t Registry::allocateSlotIndex() {
        if (!_freeList.empty()) {
            const uint32_t idx = _freeList.back();
            _freeList.pop_back();
            // slot already exists in _slots
            return idx;
        }

        // New slot
        const uint32_t idx = static_cast<uint32_t>(_slots.size());
        _slots.emplace_back();
        _generations.push_back(1); // start gen at 1 for nicer debugging
        return idx;
    }

    Registry::Slot* Registry::trySlot(RRID rid) noexcept {
        if (rid.id >= _slots.size()) return nullptr;
        if (_generations[rid.id] != rid.generation) return nullptr;
        Slot& s = _slots[rid.id];
        if (!s.alive || !s.wrapper) return nullptr;
        return &s;
    }

    const Registry::Slot* Registry::trySlot(RRID rid) const noexcept {
        if (rid.id >= _slots.size()) return nullptr;
        if (_generations[rid.id] != rid.generation) return nullptr;
        const Slot& s = _slots[rid.id];
        if (!s.alive || !s.wrapper) return nullptr;
        return &s;
    }

    Registry::Slot& Registry::slotChecked(RRID rid) {
      Slot* s = trySlot(rid);
      if (!s) {
        throw std::runtime_error("ResourceRegistry::Registry: stale or invalid RRID {id=" + std::to_string(rid.id) + ", gen=" + std::to_string(rid.generation) + "}");
      }
      return *s;
    }

    const Registry::Slot& Registry::slotChecked(RRID rid) const {
      const Slot* s = trySlot(rid);
      if (!s) {
        throw std::runtime_error("ResourceRegistry::Registry: stale or invalid RRID {id=" + std::to_string(rid.id) + ", gen=" + std::to_string(rid.generation) + "}");
      }
      return *s;
    }

    RRID Registry::getID(const std::string& name) const {
        auto it = _nameToSlot.find(name);
        if (it == _nameToSlot.end()) {
            throw std::runtime_error("ResourceRegistry::Registry::id_of: unknown resource name: " + name);
        }
        const uint32_t slotIndex = static_cast<uint32_t>(it->second);
        return RRID{ slotIndex, _generations[slotIndex] };
    }

            // Introspection
    bool Registry::exists(RRID rid) const noexcept {
        return trySlot(rid) != nullptr;
    }

    bool Registry::exists(const std::string& name) const noexcept {
        return _nameToSlot.find(name) != _nameToSlot.end();
    }

    const Registry::Slot& Registry::slotInfo(RRID rid) const {
        return slotChecked(rid);
    }

    const std::vector<Registry::Slot>& Registry::slots() const noexcept {
        return _slots;
    }
}