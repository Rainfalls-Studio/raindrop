#pragma once

#include <type_traits>
#include <unordered_map>
#include <functional>

namespace Raindrop::Core::Utils{
    template<typename Key, typename Value>
    class Registry{
        public:
            static_assert(std::is_integral<Key>::value, "Key must be an integral type");
            static_assert(std::is_unsigned<Key>::value, "Key must be an unsigned type");

            using ID = Key;
            static constexpr ID INVALID_ID = ~static_cast<ID>(0);

            Registry(){
                clear();
            }

            ~Registry() = default;

            /**
             * @brief Gets the Value associated with the given ID
             * 
             * @param id
             * @return Value& 
             */
            Value& operator[](ID id){
                return get(id);
            }

            /**
             * @brief Gets the const Value associated with the given ID
             * 
             * @param id 
             * @return const Value& 
             */
            const Value& operator[](ID id) const{
                return get(id);
            }

            /**
             * @brief Gets the Value associated with the given ID
             * 
             * @param id 
             * @return Value& 
             */
            Value& get(ID id){
                return _values[id];
            }

            /**
             * @brief Gets the const Value associated with the given ID
             * 
             * @param id 
             * @return const Value& 
             */
            const Value& get(ID id) const{
                return _values[id];
            }

            /**
             * @brief Insert a new value into the registry
             * 
             * @param value the value to insert
             * @return ID 
             */
            ID insert(const Value& value){
                ID id = createID();
                _values[id] = value;
                return id;
            }

            /**
             * @brief Checks if the ID is contained in the registry
             * 
             * @param id
             * @return true 
             * @return false 
             */
            bool contains(ID id) const{
                return _values.count(id) == 1;
            }

            /**
             * @brief Removes the value associated with the given ID
             * 
             * @param id 
             */
            void remove(ID id){
                _values.erase(id);
            }

            /**
             * @brief Removes all values from the registry
             * 
             */
            void clear(){
                _values.clear();
                _nextID = 0;
            }

            void foreach(const std::function<void(ID, const Value&)>& callback){

            }

        private:
            std::unordered_map<ID, Value> _values;
            ID _nextID;

            ID createID(){
                return _nextID++;
            }

            void freeID(ID id){}
    };
}