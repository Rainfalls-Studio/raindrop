#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace Scheduler{
    class ILoopStorage{
        public:
            virtual ~ILoopStorage() = default;
    };

    template<typename T>
    class LoopStorage : public ILoopStorage{
        public:
            template<typename... Args>
            LoopStorage(Args&&... args) : value(std::forward<Args>(args)...) {}

            virtual ~LoopStorage() override = default;
            T value;
    };

    class LoopStorageRegistry{
        public:
            template<typename T, typename... Args>
            T& emplace(Args&&... args) {
                auto ptr = std::make_unique<LoopStorage<T>>(std::forward<Args>(args)...);
                T& ref = ptr->value;

                auto key = std::type_index(typeid(T));
                _storages[key] = std::move(ptr);

                return ref;
            }
            
            template<typename T>
            T& get() const {
                auto it = _storages.find(std::type_index(typeid(T)));
                if (it == _storages.end())
                    throw std::runtime_error("Type not found");
                return static_cast<LoopStorage<T>*>(it->second.get())->value;
            }

            template<typename T, typename... Args>
            T& getOrEmplace(Args&&... args){
                auto it = _storages.find(std::type_index(typeid(T)));
                if (it == _storages.end()){
                    return emplace<T>(std::forward<Args>(args)...);
                }
                return static_cast<LoopStorage<T>*>(it->second.get())->value;
            }
            
            void clear() {
                _storages.clear();
            }

        private:
            std::unordered_map<std::type_index, std::unique_ptr<ILoopStorage>> _storages;
    };
} // namespace Raindrop::Core::Scheduler