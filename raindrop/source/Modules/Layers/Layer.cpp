#include "Raindrop/Modules/Layers/Layer.hpp"

namespace Raindrop::Layers{
    Layer::~Layer(){}

    const std::string& Layer::name() const noexcept{
        return _name;
    }

    float Layer::priority() const noexcept{
        return _priority;
    }

    void Layer::setPriority(float priority){
        std::unique_lock lock(_ctx.mtx);
        _priority = priority;

        // signal parent to update it's children list
        if (auto parent = _parent.lock()){
            parent->makeDirty_unsafe();
        }
    }

    std::shared_ptr<Layer> Layer::createChild(const std::string& name, float priority){
        std::unique_lock lock(_ctx.mtx);
        
        // make_shared will fail as the Layer's constructor is private
        auto self = shared_from_this();
        std::shared_ptr<Layer> layer = std::shared_ptr<Layer>(new Layer(name, priority, self, _ctx));

        _children.push_back(layer);
        makeDirty_unsafe();

        return layer;
    }

    void Layer::removeChild(const std::shared_ptr<Layer>& child){
        std::unique_lock lock(_ctx.mtx);
        auto it = std::find(
            _children.begin(),
            _children.end(),
            child
        );

        
        if (it != _children.end()){
            child->_parent.reset();
            _children.erase(it);
        }
    }
    
    Layer::Layer(const std::string& name, float priority, const std::shared_ptr<Layer>& parent, Context& ctx) : 
        _ctx{ctx},
        _name{name},
        _priority{priority},
        _parent{parent}
    {}

    void Layer::makeDirty_unsafe(){
        _dirty = true;
    }

    void Layer::pushSubcriber(const std::shared_ptr<SubscriberBase>& subscriber){
        std::unique_lock lock(_ctx.mtx);
        _subscribers.push_back(subscriber);

        sortSubscribers_unsafe();
    }

    void Layer::sortSubscribers_unsafe(){

        std::vector<std::shared_ptr<SubscriberBase>> sorted;
        sorted.reserve(_subscribers.size());

        auto it = _subscribers.begin();
        while (it != _subscribers.end()){
            auto subscriber = it->lock();
            if (!subscriber){
                it = _subscribers.erase(it);
                continue;
            }
                
            sorted.push_back(subscriber);
        }

        std::sort(
            sorted.begin(),
            sorted.end(),
            [](const auto& A, const auto& B) -> bool {
                return A->priority() > B->priority();
            }
        );

        _subscribers.resize(sorted.size());
        for (size_t i=0; i<sorted.size(); i++){
            _subscribers[i] = sorted[i];
        }
    }


    void Layer::transmit(const TransmissionBase& transmission){
        // shared lock because transmit doesn't cause modifications
        std::shared_lock lock(_ctx.mtx);
        transmit_unsafe(transmission);
    }

    Result Layer::transmit_unsafe(const TransmissionBase& transmission){
        auto result = signalSubsribers(transmission);

        switch (result.type){
            case Result::CONSUME: return Result::Continue(); // just stop right now

            // transmission rerouting
            case Result::REROUTE: {
                auto& reroute = result.getReroute();
                
                if (auto target = reroute.target.lock()){
                    return target->transmit_unsafe(transmission);
                }
                return Result::Continue();
            }

            default: break;
        }

        transmitChildren(transmission);
        if (result.type == Result::STOP_SIBLINGS) return result;
        return Result::Continue();
    }

    void Layer::transmitChildren(const TransmissionBase& transmission){
        for (auto children : _children){
            auto result = children->transmit_unsafe(transmission);

            if (result.type == Result::STOP_SIBLINGS) break;
        }
    }

    Result Layer::signalSubsribers(const TransmissionBase& transmission){
        for (const auto& weak : _subscribers){
            auto subscriber = weak.lock();
            if (!subscriber) continue;

            auto result = subscriber->call(transmission);

            // stop on the first original result
            if (result.type != Result::CONTINUE) return result;
        }

        return Result::Continue();
    }
}