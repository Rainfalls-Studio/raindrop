#pragma once

#include <typeindex>
#include <Raindrop/Raindrop.hpp>
#include "RRID.hpp"
#include "IResourceWrapper.hpp"
#include "detail.hpp"

namespace ResourceRegistry{
    class Registry : public Raindrop::IModule{
        public:
            struct Slot{
                RRID id;
                std::string name;
                std::type_index wrapperType{typeid(void)};
                std::type_index valueType{typeid(void)};

                std::unique_ptr<IResourceWrapper> wrapper;
                bool alive = false;
            };

            Registry() = default;


            virtual Raindrop::Result initialize(Raindrop::InitHelper& helper) override;

            template<typename Wrapper, typename... Args>
            RRID create(std::string name, Args&&... args){
                if (name.empty()) {
                    throw std::invalid_argument("ResourceRegistry::Registry::create: name is empty");
                }

                if (_nameToSlot.contains(name)) {
                    throw std::runtime_error("ResourceRegistry::Registry::create: resource name already exists: " + name);
                }

                const uint32_t slotIndex = allocateSlotIndex();
                Slot& s = _slots[slotIndex];

                // Bump generation on reuse already done by destroy(); slot has a current generation.
                s.id = RRID{ slotIndex, _generations[slotIndex] };
                s.name = std::move(name);
                s.wrapperType = std::type_index(typeid(Wrapper));
                s.valueType = detail::valueTypeIndex<Wrapper>();
                s.wrapper = std::make_unique<detail::WrapperModel<Wrapper>>(std::forward<Args>(args)...);
                s.alive = true;

                _nameToSlot.emplace(s.name, static_cast<size_t>(slotIndex));
                return s.id;
            }

            void destroy(RRID id);

            // Typed get by RRID
            template <class Wrapper>
            Wrapper& get(RRID rid) {
                Slot& s = slotChecked(rid);
                type_check_or_throw<Wrapper>(s);
                return *static_cast<Wrapper*>(s.wrapper->rawPtr());
            }

            template <class Wrapper>
            const Wrapper& get(RRID rid) const {
                const Slot& s = slotChecked(rid);
                type_check_or_throw<Wrapper>(s);
                return *static_cast<const Wrapper*>(s.wrapper->rawPtr());
            }

            // Typed get by name (debug/bootstrapping)
            template <class Wrapper>
            Wrapper& get(const std::string& name) {
                return get<Wrapper>(getID(name));
            }

            template <class Wrapper>
            const Wrapper& get(const std::string& name) const {
                return get<Wrapper>(getID(name));
            }

            // try_get variants return nullptr if not found / stale / wrong type
            template <class Wrapper>
            Wrapper* try_get(RRID rid) noexcept {
                Slot* s = trySlot(rid);
                if (!s) return nullptr;
                if (s->wrapperType != std::type_index(typeid(Wrapper))) return nullptr;
                return static_cast<Wrapper*>(s->wrapper->rawPtr());
            }

            template <class Wrapper>
            const Wrapper* try_get(RRID rid) const noexcept {
                const Slot* s = trySlot(rid);
                if (!s) return nullptr;
                if (s->wrapperType != std::type_index(typeid(Wrapper))) return nullptr;
                return static_cast<const Wrapper*>(s->wrapper->rawPtr());
            }

            // Lookup RRID by name
            RRID getID(const std::string& name) const;

            // Introspection
            bool exists(RRID rid) const noexcept;
            bool exists(const std::string& name) const noexcept;

            const Slot& slotInfo(RRID rid) const;
            const std::vector<Slot>& slots() const noexcept;
        
        private:
            std::vector<Slot> _slots;
            std::unordered_map<std::string, size_t> _nameToSlot;
            std::vector<uint32_t> _generations;
            std::vector<uint32_t> _freeList;

            uint32_t allocateSlotIndex();

            Slot* trySlot(RRID rid) noexcept;
            const Slot* trySlot(RRID rid) const noexcept;

            Slot& slotChecked(RRID rid);
            const Slot& slotChecked(RRID rid) const;


            template <class Wrapper>
            static void typeCheckOrThrow(const Slot& s) {
                const std::type_index expected = std::type_index(typeid(Wrapper));
                if (s.wrapperType != expected) {
                    throw std::runtime_error(
                    "ResourceRegistry::Registry::get: wrapper type mismatch for resource '" + s.name +
                    "'. Expected=" + detail::type_name(expected) +
                    " Actual=" + detail::type_name(s.wrapperType)
                    );
                }
            }
    };
}