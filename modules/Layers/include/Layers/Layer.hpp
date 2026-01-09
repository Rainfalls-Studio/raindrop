#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Context.hpp"
#include "Subscriber.hpp"

namespace Layers{
    class Layer : public std::enable_shared_from_this<Layer>{
        public:
            Layer(const Layer&) = delete;
            Layer(Layer&&) = delete;

            Layer& operator=(const Layer&) = delete;
            Layer& operator=(Layer&&) = delete;

            ~Layer();

            const std::string& name() const noexcept;
            float priority() const noexcept;

            void setPriority(float priority);

            std::shared_ptr<Layer> createChild(const std::string& name, float priority);
            void removeChild(const std::shared_ptr<Layer>& child);

            template<typename T>
            void transmit(const T& transmission){
                static_assert(std::is_base_of<Transmission, T>::value, "T has to be derived from Transmission<T>");
                transmit(*static_cast<const Transmission*>(&transmission));
            }

            void transmit(const Transmission& transmission);

            template<typename T, typename... Args>
            std::shared_ptr<Subscriber<T>> subscribe(Args&&... args){
                std::shared_ptr<Subscriber<T>> subscriber = std::make_shared<Subscriber<T>>(std::forward<Args>(args)...);
                pushSubcriber(std::static_pointer_cast<SubscriberBase>(subscriber));
                return subscriber;   
            }
        
        private:
            friend class LayerManager;
            Layer(const std::string& name, float priority, const std::shared_ptr<Layer>& parent, Context& ctx);
            
            Context& _ctx;
            std::string _name;
            float _priority;


            // called by children layers to signal that their priority has changed
            // and thus making the children vector 'dirty'
            void makeDirty_unsafe();

            // should the '_children' vector be resorted
            bool _dirty;


            std::weak_ptr<Layer> _parent;
            std::vector<std::shared_ptr<Layer>> _children;

            void pushSubcriber(const std::shared_ptr<SubscriberBase>& subscriber);
            void sortSubscribers_unsafe();
            std::vector<std::weak_ptr<SubscriberBase>> _subscribers;

            Result transmit_unsafe(const Transmission& transmission);
            Result signalSubsribers(const Transmission& transmission);
            void transmitChildren(const Transmission& transmission);
    };
}