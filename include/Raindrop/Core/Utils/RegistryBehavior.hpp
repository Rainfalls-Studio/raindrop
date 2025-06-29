#pragma once

namespace Raindrop::Core::Utils{
    template<typename Key, typename Value>
    class RegistryBehavior{
        public:
            using ID = Key;

            virtual ~RegistryBehavior() = default;

            virtual void onCreation(ID id, const Value& value) = 0;
            virtual void onDestruction(ID id, const Value& value) = 0;
    };
}