#pragma once

#include <stdexcept>
#include <type_traits>
#include <unordered_map>

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

            virtual ~Registry() = default;

            /**
             * @brief Gets the Value associated with the given ID
             * 
             * @param id
             * @return Value& 
             */
            virtual Value& operator[](ID id){
                return get(id);
            }

            /**
             * @brief Gets the const Value associated with the given ID
             * 
             * @param id 
             * @return const Value& 
             */
            virtual const Value& operator[](ID id) const{
                return get(id);
            }

            /**
             * @brief Gets the Value associated with the given ID
             * 
             * @param id 
             * @return Value& 
             */
            virtual Value& get(ID id){
                auto it = _values.find(id);
                if (it == _values.end()){
                    throw std::out_of_range("Registry::get: ID not found: " + std::to_string(id));
                }
                return it->second;
            }

            /**
             * @brief Gets the const Value associated with the given ID
             * 
             * @param id 
             * @return const Value& 
             */
            virtual const Value& get(ID id) const{
                auto it = _values.find(id);
                if (it == _values.end()){
                    throw std::out_of_range("Registry::get: ID not found: " + std::to_string(id));
                }
                return it->second;
            }

            /**
             * @brief Insert a new value into the registry
             * 
             * @param value the value to insert
             * @return ID 
             */
            virtual ID insert(const Value& value){
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
            virtual bool contains(ID id) const{
                return _values.count(id) == 1;
            }

            /**
             * @brief Removes the value associated with the given ID
             * 
             * @param id 
             */
            virtual void remove(ID id){
                _values.erase(id);
            }

            /**
             * @brief Removes all values from the registry
             * 
             */
            virtual void clear(){
                _values.clear();
                _nextID = 0;
            }

        protected:
            std::unordered_map<ID, Value> _values;
            ID _nextID;

            ID createID(){
                return _nextID++;
            }
    };
}