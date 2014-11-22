#include "graph.h"
#include "utils.h"
#include "nodes/uniform_buffer.h"
#include "nodes/attribute_buffer.h"
#include "graph_executor.h"

namespace noises
{
    Graph::Graph() : id_counter_(0) { }

    int Graph::add_node(std::unique_ptr<GraphNode> node)
    {
        node->set_id(id_counter_);
        node->set_parent(*this);
        id_counter_++;

        GraphNode& node_ref = *node;

        nodes_.push_back(std::move(node));

        node_ref.request_recalculate_sockets();
        return id_counter_ - 1;
    }

    void Graph::remove_node(GraphNode* node)
    {
        // Disconnect all connections to this node
        for(auto& socket : node->inputs().all_sockets())
            disconnect(socket);

        for(auto& socket : node->outputs().all_sockets())
            disconnect_all(socket);

        utils::remove_by_pointer(nodes_, node);
    }

    std::vector<GraphNode*> Graph::get_nodes_by_node_name(const std::string &name) const
    {
        std::vector<GraphNode*> return_list;
        for(auto& node : nodes_)
        {
            if(node->node_name() == name)
                return_list.push_back(node.get());
        }
        return return_list;
    }

    boost::optional<std::reference_wrapper<GraphNode>> Graph::get_node_by_name(const std::string &name) const
    {
        for(auto& node : nodes_)
        {
            if(node->name() == name)
                return std::ref(*node);
        }
        return boost::none;
    }

    boost::optional<std::reference_wrapper<GraphNode>> Graph::get_node_by_id(int id) const
    {
        for(auto& node : nodes_)
        {
            if(node->id() == id)
                return std::ref(*node);
        }

        for(auto& node : input_nodes_)
        {
            if(node->id() == id)
                return std::ref(*node);
        }

        for(auto& node : output_nodes_)
        {
            if(node->id() == id)
                return std::ref(*node);
        }

        return boost::none;
    }

    void Graph::connect(OutputSocket& output, InputSocket& input)
    {
        if(input.connection() != nullptr)
            disconnect(input);

        std::unique_ptr<Connection> connection(new Connection(input, output, *this));
        output.add_connection(*connection);
        input.set_connection(*connection);

        connection->set_id(id_counter_);
        id_counter_++;

        connections_.push_back(std::move(connection));
    }

    void Graph::disconnect(InputSocket& input)
    {
        auto possible_connection = input.connection();
        if(!possible_connection)
            return;

        //Could iterate connection_ to find same non-const object but a const cast is simpler
        Connection& connection = const_cast<Connection&>(possible_connection->get());
        connection.disconnect();
        remove_connection(connection);
    }

    void Graph::disconnect_all(OutputSocket& output)
    {
        for(auto& connection_wrapper : output.connections())
        {
            //Could iterate connection_ to find same non-const object but a const cast is simpler

            Connection& connection = const_cast<Connection&>(connection_wrapper.get());
            connection.disconnect();
            remove_connection(connection);
        }
    }

    void Graph::remove_connection(const Connection& connection)
    {
        utils::remove_by_pointer(connections_, &connection);
    }

    void Graph::remove_property(const std::string &name)
    {
        properties_.remove(name);
    }

    boost::optional<std::reference_wrapper<Property>> Graph::get_property_by_name(const std::string &name)
    {
        return properties_.get_property_by_name(name);
    }

    // Icky repetitive
    GraphNode& Graph::add_attribute_output(const std::string &name)
    {
        if(get_attribute_output(name) || get_uniform_output(name))
            throw std::invalid_argument(name + " is already taken as an output.");

        std::unique_ptr<GraphNode> buffer_node(new nodes::AttributeBuffer);
        buffer_node->set_name(name);
        buffer_node->set_id(id_counter_);
        buffer_node->set_is_graph_internal_node(true);
        buffer_node->input("Input").set_accepts(ConnectionDataType::any());
        id_counter_++;

        GraphNode& buffer_node_ref = *buffer_node;
        output_nodes_.push_back(std::move(buffer_node));
        return buffer_node_ref;
    }

    GraphNode& Graph::add_uniform_output(const std::string &name)
    {
        if(get_uniform_output(name) || get_attribute_output(name))
            throw std::invalid_argument(name + " is already taken as an output.");

        std::unique_ptr<GraphNode> buffer_node(new nodes::UniformBuffer);
        buffer_node->set_name(name);
        buffer_node->set_id(id_counter_);
        buffer_node->set_is_graph_internal_node(true);
        buffer_node->input("Input").set_accepts(ConnectionDataType::any());
        id_counter_++;

        GraphNode& buffer_node_ref = *buffer_node;
        output_nodes_.push_back(std::move(buffer_node));
        return buffer_node_ref;
    }

    GraphNode& Graph::add_uniform_input(const std::string &name)
    {
        if(get_uniform_input(name) || get_attribute_input(name))
            throw std::invalid_argument(name + " is already taken as an input.");

        std::unique_ptr<GraphNode> buffer_node(new nodes::UniformBuffer);
        buffer_node->set_name(name);
        buffer_node->set_id(id_counter_);
        buffer_node->set_is_graph_internal_node(true);
        buffer_node->input("Input").set_optional(true);
        id_counter_++;

        GraphNode& buffer_node_ref = *buffer_node;
        input_nodes_.push_back(std::move(buffer_node));
        return buffer_node_ref;
    }

    GraphNode& Graph::add_attribute_input(const std::string &name)
    {
        if(get_uniform_input(name) || get_attribute_input(name))
            throw std::invalid_argument(name + " is already taken as an input.");

        std::unique_ptr<GraphNode> buffer_node(new nodes::AttributeBuffer);
        buffer_node->set_name(name);
        buffer_node->set_id(id_counter_);
        buffer_node->set_is_graph_internal_node(true);
        buffer_node->input("Input").set_optional(true);
        id_counter_++;

        GraphNode& buffer_node_ref = *buffer_node;
        input_nodes_.push_back(std::move(buffer_node));
        return buffer_node_ref;
    }

    boost::optional<std::reference_wrapper<nodes::AttributeBuffer>> Graph::get_attribute_input(const std::string &name)
    {
        return get_buffer<nodes::AttributeBuffer>(name, input_nodes_);
    }

    boost::optional<std::reference_wrapper<nodes::AttributeBuffer>> Graph::get_attribute_output(const std::string &name)
    {
        return get_buffer<nodes::AttributeBuffer>(name, output_nodes_);
    }

    boost::optional<std::reference_wrapper<nodes::UniformBuffer>> Graph::get_uniform_input(const std::string &name)
    {
        return get_buffer<nodes::UniformBuffer>(name, input_nodes_);
    }

    boost::optional<std::reference_wrapper<nodes::UniformBuffer>> Graph::get_uniform_output(const std::string &name)
    {
        return get_buffer<nodes::UniformBuffer>(name, output_nodes_);
    }

    //Below be boring getters

    std::vector<std::reference_wrapper<Connection>> Graph::connections()
    {
        return utils::to_reference_array(connections_);
    }

    const std::vector<std::reference_wrapper<const Connection>> Graph::connections() const
    {
        return utils::to_reference_array_const(connections_);
    }

    std::vector<std::reference_wrapper<GraphNode>> Graph::nodes()
    {
        return utils::to_reference_array(nodes_);
    }

    const std::vector<std::reference_wrapper<const GraphNode>> Graph::nodes() const
    {
        return utils::to_reference_array_const(nodes_);
    }

    std::vector<std::reference_wrapper<GraphNode>> Graph::output_nodes()
    {
        return utils::to_reference_array(output_nodes_);
    }

    const std::vector<std::reference_wrapper<const GraphNode>> Graph::output_nodes() const
    {
        return utils::to_reference_array_const(output_nodes_);
    }

    std::vector<std::reference_wrapper<GraphNode>> Graph::input_nodes()
    {
        return utils::to_reference_array(input_nodes_);
    }

    const std::vector<std::reference_wrapper<const GraphNode>> Graph::input_nodes() const
    {
        return utils::to_reference_array_const(input_nodes_);
    }
    
    void Graph::set_input_uniform_raw(const std::string &input_name, const ConnectionDataType &data_type, const unsigned char *data_ptr)
    {
        // Make sure there's an input node with [input_name] and it's a uniform type.
        auto node_it = std::find_if(input_nodes_.begin(), input_nodes_.end(), [input_name](std::unique_ptr<GraphNode>& node) { return node->name() == input_name; });
        if(node_it == input_nodes_.end())
            throw std::invalid_argument(input_name + " is not a valid input.");
        if((**node_it).output("Output").type() != SocketType::uniform)
            throw std::invalid_argument(input_name + " is not a uniform argument.");

        auto existing_it = manual_input_buffers_.find(input_name);
        if(existing_it != manual_input_buffers_.end())
            manual_input_buffers_.erase(existing_it);
        
        std::unique_ptr<DataBuffer> new_buffer(new DataBuffer(0));

        new_buffer->add_uniform(data_type);
        new_buffer->set_uniform_raw(0, data_type.size_full(), data_ptr);

        manual_input_buffers_.insert(std::pair<std::string, std::unique_ptr<DataBuffer>>(input_name, std::move(new_buffer)));

        nodes::UniformBuffer& input_node = *this->get_uniform_input(input_name);
        input_node.set_output_type(data_type);
        input_node.input().set_accepts(data_type);

        refresh_all_sockets();
    }
    
    void Graph::set_input_attribute_raw(const std::string &input_name, const ConnectionDataType &data_type, const unsigned char *data_ptr, DataBuffer::size_type attribute_length)
    {
        auto node_it = std::find_if(input_nodes_.begin(), input_nodes_.end(), [input_name](std::unique_ptr<GraphNode>& node) { return node->name() == input_name; });
        if(node_it == input_nodes_.end())
            throw std::invalid_argument(input_name + " is not a valid input.");
        if((**node_it).output("Output").type() != SocketType::attribute)
            throw std::invalid_argument(input_name + " is not an attribute type.");

        auto existing_it = manual_input_buffers_.find(input_name);
        if(existing_it != manual_input_buffers_.end())
            manual_input_buffers_.erase(existing_it);

        std::unique_ptr<DataBuffer> new_buffer(new DataBuffer(attribute_length));

        new_buffer->add_attribute(data_type);
        new_buffer->set_attribute_all_raw(0, data_ptr, attribute_length * data_type.size_full());

        manual_input_buffers_.insert(std::pair<std::string, std::unique_ptr<DataBuffer>>(input_name, std::move(new_buffer)));

        AttributeInfo attribute_info(attribute_length, "unknown"); //todo

        nodes::AttributeBuffer& input_node = *this->get_attribute_input(input_name);
        input_node.set_output_type(data_type, attribute_info);
        input_node.input().set_accepts(data_type);

        refresh_all_sockets();
    }

    boost::optional<std::reference_wrapper<const DataBuffer>> Graph::get_input_buffer(const std::string &input_name) const
    {
        auto it = manual_input_buffers_.find(input_name);
        if(it == manual_input_buffers_.end())
            return boost::none;

        return std::ref(static_cast<const DataBuffer&>(*std::get<1>(*it)));
    }

    GraphOutputs Graph::execute() const
    {
        GraphExecutor executor(*this);
        return executor.execute();
    }

    void Graph::refresh_all_sockets()
    {
        for(GraphNode& node : nodes())
        {
            node.request_recalculate_sockets();
        }

        for(GraphNode& node : input_nodes())
        {
            node.request_recalculate_sockets();
        }

        for(GraphNode& node : output_nodes())
        {
            node.request_recalculate_sockets();
        }
    }
}
