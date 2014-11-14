#include "graph_node.h"

namespace noises
{
    GraphNode::GraphNode() : name_(""), attribute_override_(AttributeInfo::inherit()) { }

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
        // Optionally implemented in derived classes
    }

    void GraphNode::execute_attributes(const CompositeDataBuffer &, DataBuffer &, DataBuffer::size_type) const
    {
        // Optionally implemented in derived classes
    }

    int GraphNode::id() const
    {
        return id_;
    }

    void GraphNode::set_id(int id)
    {
        id_ = id;
    }

    boost::optional<std::reference_wrapper<Property>> GraphNode::get_property_by_name(const std::string &name)
    {
        return properties_.get_property_by_name(name);
    }

    boost::optional<std::reference_wrapper<const Property>> GraphNode::get_property_by_name(const std::string &name) const
    {
        return properties_.get_property_by_name(name);
    }

    void GraphNode::remove_property(const Property &property)
    {
        properties_.remove(property.name());
    }
    
    const std::vector<std::reference_wrapper<Property>> GraphNode::properties()
    {
        return properties_.properties();
    }
    
    const std::vector<std::reference_wrapper<const Property>> GraphNode::properties() const
    {
        return properties_.properties();
    }

    void GraphNode::override_attribute_info(AttributeInfo info)
    {
        attribute_override_ = info;
        recalculate_sockets();
    }

    void GraphNode::recalculate_sockets_internal()
    {
        // Prevents infinite recursion
        if(in_recalculate_sockets_)
            return;
        in_recalculate_sockets_ = true;
        recalculate_sockets();
        in_recalculate_sockets_ = false;
    }

    void GraphNode::recalculate_sockets()
    {
        // Optionally implemented in derived classes
    }
}

