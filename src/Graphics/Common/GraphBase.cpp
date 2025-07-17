#include "Raindrop/Graphics/Common/GraphBase.hpp"

namespace Raindrop::Graphics::Common{
    GraphBase::GraphBase(){}

    void GraphBase::addNode(const std::shared_ptr<INode>& node){
        NodeInfo info{
            .node = node,
            .inputs = node->inputs(),
            .outputs = node->outputs()
        };

        _nodes.push_back(info);
    }

    void GraphBase::removeNode(const std::shared_ptr<INode>& node){
        // std::remove_if(info)
        _nodes.remove_if(
            [node](const NodeInfo& info) -> bool {
                return info.node == node;
            }
        );
    }

    void GraphBase::bind(const std::shared_ptr<INode>& srcNode, const std::shared_ptr<INode>& dstNode, uint32_t srcResourceSlot, uint32_t dstResourceSlot){
        BindingInfo info{
            .srcNode = srcNode,
            .dstNode = dstNode,
            .srcResourceSlot = srcResourceSlot,
            .dstResourceSlot = dstResourceSlot
        };

        _bindings.push_back(info);
    }
}