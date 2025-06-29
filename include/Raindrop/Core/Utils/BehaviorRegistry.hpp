#pragma once

#include "Raindrop/Core/Utils/Registry.hpp"
#include "Raindrop/Core/Utils/RegistryBehavior.hpp"
#include <list>
#include <memory>

namespace Raindrop::Core::Utils{
    template<typename Key, typename Value>
    class BehaviorRegistry : public Registry<Key, Value>{
        using Super = Registry<Key, Value>;
        using Behavior = RegistryBehavior<Key, Value>;

        public:
            using Super::Registry;
            using ID = typename Super::ID;

            virtual ID insert(const Value& value) override{
                ID id = Super::insert(value);
                Value& v = Super::get(id);

                for(auto& behavior : _behaviors){
                    behavior->onCreation(id, v);
                }

                return id;
            }

            virtual void remove(ID id) override{
                Value& value = Super::get(id);
                for (auto& behavior : _behaviors){
                    behavior->onDestruction(id, value);
                }

                Super::remove(id);
            }

            void addBehavior(std::shared_ptr<Behavior> behavior){
                _behaviors.push_back(std::move(behavior));
            }

        private:
            std::list<std::shared_ptr<Behavior>> _behaviors;
    };
}