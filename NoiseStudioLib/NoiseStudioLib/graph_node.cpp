#include "graph_node.h"

namespace noises
{
    GraphNode::GraphNode() : name_("") { }

    GraphNode::~GraphNode() { }

    const std::string& GraphNode::name() const
    {
        return name_;
    }

    void GraphNode::set_name(const std::string &name)
    {
        name_ = name;
    }

    const SocketCollection<InputSocket>& GraphNode::inputs() const
    {
        return inputs_;
    }

    const SocketCollection<OutputSocket>& GraphNode::outputs() const
    {
        return outputs_;
    }

    SocketCollection<InputSocket>& GraphNode::inputs()
    {
        return inputs_;
    }

    SocketCollection<OutputSocket>& GraphNode::outputs()
    {
        return outputs_;
    }

    void GraphNode::execute_uniforms(const CompositeDataBuffer &, DataBuffer &) const
    {

    }

    void GraphNode::execute_attributes(const CompositeDataBuffer &, DataBuffer &, DataBuffer::size_type) const
    {

    }

    int GraphNode::id() const
    {
        return id_;
    }

    void GraphNode::set_id(int id)
    {
        id_ = id;
    }
}

