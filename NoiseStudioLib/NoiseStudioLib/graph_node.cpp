#include "graph_node.h"

#include <functional>

#include "validation_results.h"

namespace noises
{
    GraphNode::GraphNode() :
        name_(""),
        inputs_(this),
        outputs_(this),
        in_recalculate_sockets_(false),
        is_graph_internal_node_(false),
        parent_(nullptr)
    {
        inputs_.listen_socket_changed([this](const InputSocket&) { recalculate_sockets_internal(); });
        outputs_.listen_socket_changed([this](const OutputSocket&) { recalculate_sockets_internal(); });

        properties_.listen_changed([this](Property&) { recalculate_sockets_internal(); });
    }

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

    std::string GraphNode::display_name() const
    {
        if(name() == "")
            return node_name() + " (Unnamed)";
        return name() + " (" + node_name() + ")";
    }

    InputSocket& GraphNode::input(const std::string &name)
    {
        return inputs_[name];
    }

    const InputSocket& GraphNode::input(const std::string &name) const
    {
        return inputs_[name];
    }

    OutputSocket& GraphNode::output(const std::string &name)
    {
        return outputs_[name];
    }

    const OutputSocket& GraphNode::output(const std::string &name) const
    {
        return outputs_[name];
    }

    void GraphNode::request_recalculate_sockets()
    {
        recalculate_sockets_internal();
    }

    bool GraphNode::is_graph_internal_node() const
    {
        return is_graph_internal_node_;
    }

    void GraphNode::set_is_graph_internal_node(bool is_internal)
    {
        is_graph_internal_node_ = is_internal;
    }

    Property& GraphNode::property(const std::string& name)
    {
        return const_cast<Property&>(static_cast<const GraphNode*>(this)->property(name));
    }

    const Property& GraphNode::property(const std::string& name) const
    {
        auto potential_property = get_property_by_name(name);
        if(!potential_property)
            throw std::invalid_argument("No such property " + name);
        return potential_property->get();
    }

    void GraphNode::validate(ValidationResults&) const
    {

    }

    void GraphNode::set_parent(Graph &parent)
    {
        parent_ = &parent;
    }

    Graph* GraphNode::parent()
    {
        return parent_;
    }

    const Graph* GraphNode::parent() const
    {
        return parent_;
    }
}

