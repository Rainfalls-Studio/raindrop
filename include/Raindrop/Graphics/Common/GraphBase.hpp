#pragma once

#include "INode.hpp"
#include <memory>
#include <list>

namespace Raindrop::Graphics::Common{
    class GraphBase{
        public:
            GraphBase();

            void addNode(const std::shared_ptr<INode>& node);
            void removeNode(const std::shared_ptr<INode>& node);

            void bind(const std::shared_ptr<INode>& srcNode, const std::shared_ptr<INode>& dstNode, uint32_t srcResourceSlot, uint32_t dstResourceSlot);

        private:
            struct BindingInfo{
                std::weak_ptr<INode> srcNode;
                std::weak_ptr<INode> dstNode;
                uint32_t srcResourceSlot;
                uint32_t dstResourceSlot;
            };

            struct NodeInfo{
                std::shared_ptr<INode> node;

                // caching to avoid constructing the node slots over and over
                std::vector<ResourceDescription> inputs;
                std::vector<ResourceDescription> outputs;
            };

            std::list<NodeInfo> _nodes;
            std::list<BindingInfo> _bindings;
    };
}