/**
 * @brief this file contain the layer stack class.
 * @warning DO NOT CHANGE ANYTHING IF YOU NOT KNOWN WHAT YOU ARE DOING
 * @attention IF YOU DO ANY CHANGE IN THIS FILE, MAKE SURE THAT THE WHOLE PROJECT IS RECOMPILED
 * @authors @Aalexdev (aaleex3984@gmail.com), ... (add here)
 */

/**
 * MIT License
 * Copyright (c) 2022 Rainfalls studio
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#ifndef __RAINDROP_LAYERS_LAYER_STACK_HPP__
#define __RAINDROP_LAYERS_LAYER_STACK_HPP__

#include "core.hpp"
#include "Layer.hpp"
#include "FrameData.hpp"

namespace rnd::layers{
	class LayerStack{
		public:
			LayerStack() = default;
			~LayerStack() = default;
			
			void init();
			void shutdown();

			template<typename T, typename... Args>
			T& pushLayer(float priority = 1.f, const Args&... args){
				PROFILE_FUNCTION();
				#if RND_ASSERTS
					bool isBaseOf = (bool)std::is_base_of<Layer, T>();
					RND_ASSERT(isBaseOf, "cannot push a layer if it's not a derived class of rnd::layers::Layer");
				#endif

				LayerNode node;
				T* layer = (T*)memory::malloc(sizeof(T));
				new (layer) T(args...);

				layer->uid = nextID;
				nextID++;

				node.layer = (Layer*)layer;
				node.priority = priority;

				layers.insert(node);
				return *layer;
			}

			void clear();

			void update(const FrameData& data);
		
		private:
			struct LayerNode{
				Layer* layer;
				float priority = 1.f;
			};

			LUID nextID = 0;

			friend bool operator<(const LayerNode& a, const LayerNode& b);
			friend bool operator>(const LayerNode& a, const LayerNode& b);

			Set<LayerNode, std::greater<LayerNode>> layers;
	};

	bool operator<(const rnd::layers::LayerStack::LayerNode& a, const rnd::layers::LayerStack::LayerNode& b){
		return a.priority < b.priority;
	}

	bool operator>(const rnd::layers::LayerStack::LayerNode& a, const rnd::layers::LayerStack::LayerNode& b){
		return a.priority > b.priority;
	}
}


#endif