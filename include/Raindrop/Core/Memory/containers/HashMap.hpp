#ifndef __RAINDROP_CORE_MEMORY_HASH_MAP_HPP__
#define __RAINDROP_CORE_MEMORY_HASH_MAP_HPP__

#include <Core/Memory/allocators/Allocator.hpp>
#include <Core/Memory/containers/Array.hpp>
#include <Core/Debug/profiler.hpp>
#include <Tools/hash.hpp>
#include <Tools/comparator.hpp>

namespace Raindrop::Core::Memory{
	template<class K, class V>
	class HashMap{
		public:
			HashMap(Allocator& allocator);
			HashMap(){}

			void initialize(Allocator& allocator);

			bool has(K key) const;

			// Returns the value stored for the specified key, or default if the key
			// does not exist in the hash.
			const V& lookup(K key, const V& d = {}) const;

			V* get(K key);

			void insert(K key, const V& value);

			bool remove(K key);

			void reserve(usize size);

			void clear();

			usize size() const;

			//using Iterator = usize;

			struct Iterator{
				usize index;
				K      key;
				V*     value;
			};

			Iterator begin();
			Iterator next(const Iterator& it);
			Iterator end();

			V* operator[](K key){return get(key);}

		private:
			void rehash(usize size);

			enum State : uint8{
				EMPTY,
				FILLED,
				REMOVED
			};

			struct Bucket{
				uint8 hash : 6;
				uint8 state : 2;
			};

			static_assert(sizeof(Bucket) == sizeof(uint8), "Check Bucket struct size");

			Allocator* _allocator;
			Array<Bucket> _buckets;
			Array<K> _keys;
			Array<V> _values;

			usize        _size; //Number of used entries in the table
	};

	template<class K, class V>
	HashMap<K, V>::HashMap(Allocator& allocator) : _allocator(&allocator), _buckets(allocator), _keys(allocator), _values(allocator), _size(0){
		RAINDROP_profile_function();
	}


	template<class K, class V>
	bool HashMap<K, V>::has(K key) const{
		RAINDROP_profile_function();
		usize size  = _buckets.size();

		if (size == 0)
			return false;

		ID64 _hash = Tools::hash64<K>(key);
		usize start = _hash % size;
		uint8 hash6 = static_cast<uint8>(_hash) & 0b00111111;

		// Search the buckets until we hit an empty one
		for (usize i = start; i < size; ++i){
			const Bucket* bucket = &_buckets[i];

			switch(bucket->state){
				case State::EMPTY: return false;
				case State::FILLED:
					if (hash6 == bucket->hash && Tools::equal<K>(_keys[i], key))
						return true;
					break;
				default: break;
			}
		}
		for (usize i = 0; i < start; ++i){
			const Bucket* bucket = &_buckets[i];

			switch(bucket->state){
				case State::EMPTY: return false;
				case State::FILLED:
					if (hash6 == bucket->hash && Tools::equal<K>(_keys[i], key))
						return true;
					break;
				default: break;
			}
		}

		return false;
	}

	template<class K, class V>
	const V& HashMap<K, V>::lookup(K key, const V& value) const{
		RAINDROP_profile_function();
		usize size = _buckets.size();

		if (size == 0)
			return value;

		ID64 _hash = Tools::hash64<K>(key);
		usize start = _hash % size;
		uint8 hash6 = static_cast<uint8>(_hash) & 0b00111111;

		// Search the buckets until we hit an empty one
		for (usize i = start; i < size; ++i){
			const Bucket* bucket = &_buckets[i];

			switch(bucket->state){
				case State::EMPTY: return value;
				case State::FILLED:
					if (hash6 == bucket->hash && Tools::equal<K>(_keys[i], key))
						return _values[i];
					break;
				default: break;
			}
		}

		for (usize i = 0; i < start; ++i){
			const Bucket* bucket = &_buckets[i];

			switch(bucket->state){
				case State::EMPTY: return value;
				case State::FILLED:
					if (hash6 == bucket->hash && Tools::equal<K>(_keys[i], key))
						return _values[i];
					break;
				default: break;
			}
		}

		return value;
	}

	template<class K, class V>
	V* HashMap<K, V>::get(K key){
		RAINDROP_profile_function();
		usize size = _buckets.size();

		if (size == 0) return nullptr;

		ID64 _hash = Tools::hash64<K>(key);
		usize start = _hash % size;
		uint8 hash6 = static_cast<uint8>(_hash) & 0b00111111;

		// Search the buckets until we hit an empty one
		for (usize i = start; i < size; ++i){
			const Bucket* bucket = &_buckets[i];

			switch(bucket->state){
				case State::EMPTY: return nullptr;
				case State::FILLED:
					if (hash6 == bucket->hash && Tools::equal<K>(_keys[i], key))
						return &_values[i];
					break;
				default: break;
			}
		}

		for (usize i = 0; i < start; ++i){
			const Bucket* bucket = &_buckets[i];

			switch(bucket->state){
				case State::EMPTY: return nullptr;
				case State::FILLED:
					if (hash6 == bucket->hash && Tools::equal<K>(_keys[i], key))
						return &_values[i];
					break;
				default: break;
			}
		}

		return nullptr;
	}

	template<class K, class V>
	void HashMap<K, V>::insert(K key, const V& value){
		RAINDROP_profile_function();
		usize buckets_size = _buckets.size();

		// Resize larger if the load factor goes over 2/3
		if (_size * 3 >= buckets_size * 2){
			rehash(buckets_size * 2 + 8);

			buckets_size = _buckets.size();
		}
		
		
		ID64 _hash = Tools::hash64<K>(key);
		usize start = _hash % buckets_size;
		uint8 hash6 = static_cast<uint8>(_hash) & 0b00111111;

		// Search for an unused bucket
		Bucket* bucket      = nullptr;
		usize bucket_index = 0;

		for (usize i = start; i < buckets_size; ++i){
			Bucket* b = &_buckets[i];

			if (b->state != State::FILLED){
				bucket       = b;
				bucket_index = i;
				break;
			}
		}

		if (bucket == nullptr){
			for (usize i = 0; i < start; ++i){
				Bucket* b = &_buckets[i];

				if (b->state != State::FILLED){
					bucket       = b;
					bucket_index = i;
					break;
				}
			}
		}

		RAINDROP_assert(bucket != nullptr);

		// Store the hash, key, and value in the bucket
		bucket->hash          = hash6;
		bucket->state         = State::FILLED;
		_keys[bucket_index]   = key;
		_values[bucket_index] = value;

		++_size;
	}

	template<class K, class V>
	bool HashMap<K, V>::remove(K key){
		RAINDROP_profile_function();
		usize size = _buckets.size();

		if (size == 0) return false;

		ID64 _hash = Tools::hash64<K>(key);
		usize start = _hash % size;
		uint8 hash6 = static_cast<uint8>(_hash) & 0b00111111;

		// Search the buckets until we hit an empty one
		for (usize i = start; i < size; ++i){
			Bucket* b = &_buckets[i];

			switch(b->state){
				case State::EMPTY: return false;
				case State::FILLED:
					if (b->hash == hash6 && Tools::equal<K>(_keys[i], key))
					{
						b->hash = 0;
						b->state = State::REMOVED;
						--_size;
						return true;
					}
					break;
				default: break;
			}
		}

		for (usize i = 0; i < start; ++i){
			Bucket* b = &_buckets[i];

			switch(b->state){
				case State::EMPTY: return false;
				case State::FILLED:
					if (b->hash == hash6 && Tools::equal<K>(_keys[i], key))
					{
						b->hash = 0;
						b->state = State::REMOVED;
						--_size;
						return true;
					}
					break;
				default: break;
			}
		}

		return false;
	}

	template<class K, class V>
	void HashMap<K, V>::reserve(usize size){
		RAINDROP_profile_function();
		rehash(size);
	}

	template<class K, class V>
	void HashMap<K, V>::clear(){
		RAINDROP_profile_function();
		_buckets.clear();
		_keys.clear();
		_values.clear();
		_size = 0;
	}

	template<class K, class V>
	void HashMap<K,V>::rehash(usize size){
		RAINDROP_profile_function();
		// Can't rehash down to smaller than current size or initial size
		//bucketCountNew = std::max(std::max(bucketCountNew, size),
		//	usize(s_hashTableInitialSize));

		size = size > _size ? size : _size;

		// Build a new set of buckets, keys, and values
		Array<Bucket> buckets_new(*_allocator, size);
		Array<K> keys_new(*_allocator, size);
		Array<V> values_new(*_allocator, size);

		Bucket temp;
		temp.state = State::EMPTY;

		buckets_new.resize(size, temp);
		keys_new.resize(size);
		values_new.resize(size);

		// Walk through all the current elements and insert them into the new buckets
		for (usize i = 0, end = _buckets.size(); i < end; ++i){
			Bucket* b = &_buckets[i];

			if (b->state != State::FILLED) continue;

			K key = _keys[i];

			// Hash the key and find the starting bucket
			
			ID64 _hash = Tools::hash64<K>(key);

			usize start = _hash % size;
			uint8 hash6 = static_cast<uint8>(_hash) & 0b00111111;

			// Search for an unused bucket
			Bucket* bucket      = nullptr;
			usize bucket_index = 0;

			for (usize j = start; j < size; ++j){
				Bucket* b = &buckets_new[j];

				if (b->state != State::FILLED){
					bucket       = b;
					bucket_index = j;
					break;
				}
			}

			if (bucket == nullptr){
				for (usize j = 0; j < start; ++j){
					Bucket* b = &buckets_new[j];

					if (b->state != State::FILLED)
					{
						bucket       = b;
						bucket_index = j;
						break;
					}
				}
			}

			RAINDROP_assert(bucket != nullptr);

			// Store the hash, key, and value in the bucket
			bucket->hash            = hash6;
			bucket->state           = State::FILLED;
			keys_new[bucket_index]   = key;
			values_new[bucket_index] = std::move(_values[i]);
		}

		// Swap the new buckets, keys, and values into place
		using std::swap;

		swap(_buckets, buckets_new);
		swap(_keys, keys_new);
		swap(_values, values_new);
	}

	template<class K, class V>
	typename HashMap<K, V>::Iterator HashMap<K, V>::begin(){
		RAINDROP_profile_function();
		usize i         = 0;
		const usize end = _buckets.size();

		for (; i < end; ++i){
			if (_buckets[i].state == State::FILLED)
				break;
		}

		if (i == end)
			return{ end, 0, nullptr };

		return{ i, _keys[i], &_values[i] };
	}

	template<class K, class V>
	typename HashMap<K, V>::Iterator HashMap<K, V>::next(const Iterator& it){
		RAINDROP_profile_function();
		usize i         = it.index + 1;
		const usize end = _buckets.size();

		for (; i < end; ++i){
			if (_buckets[i].state == State::FILLED) break;
		}

		if (i == end) return{ end, 0, nullptr };

		return{ i, _keys[i], &_values[i] };
	}

	template<class K, class V>
	typename HashMap<K, V>::Iterator HashMap<K, V>::end(){
		RAINDROP_profile_function();
		return{ _buckets.size(), 0, nullptr };
	}

	template<class K, class V>
	usize HashMap<K, V>::size() const{
		RAINDROP_profile_function();
		return _size;
	}
}

#endif