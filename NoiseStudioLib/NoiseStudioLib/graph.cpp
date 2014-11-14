#include "graph.h"
#include "utils.h"

namespace noises
{
    Graph::Graph() : id_counter_(0) { }

    int Graph::add(std::unique_ptr<GraphNode> node)
    {
        node->set_id(id_counter_);
        id_counter_++;

        nodes_.push_back(std::move(node));
        return id_counter_ - 1;
    }

    void Graph::remove(GraphNode* node)
    {
        // Disconnect all connections to this node
        for(auto& socket : node->inputs().attribute_sockets())
            disconnect(*socket);

        for(auto& socket : node->inputs().uniform_sockets())
            disconnect(*socket);

        for(auto& socket : node->outputs().attribute_sockets())
            disconnect_all(*socket);

        for(auto& socket : node->outputs().uniform_sockets())
            disconnect_all(*socket);

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
        return boost::none;
    }

    void Graph::connect(OutputSocket& output, InputSocket& input)
    {
        if(input.connection() != nullptr)
            disconnect(input);

        std::unique_ptr<Connection> connection(new Connection(input, output));
        output.add_connection(*connection.get());
        input.set_connection(connection.get());

        connections_.push_back(std::move(connection));
    }

    void Graph::disconnect(InputSocket& input)
    {
        if(input.connection() == nullptr)
            return;

        //Could iterate connection_ to find same non-const object but a const cast is simpler
        Connection* connection = const_cast<Connection*>(input.connection());
        connection->disconnect();
        remove_connection(connection);
    }

    void Graph::disconnect_all(OutputSocket& output)
    {
        for(auto& connection_wrapper : output.connections())
        {
            //Could iterate connection_ to find same non-const object but a const cast is simpler
            Connection* connection = const_cast<Connection*>(&connection_wrapper.get());
            connection->disconnect();
            remove_connection(connection);
        }
    }

    void Graph::remove_connection(const Connection* connection)
    {
        auto it = connections_.begin();
        for(; it != connections_.end(); ++it)
        {
            if(it->get() == connection)
                break;
        }
        if(it != connections_.end())
            connections_.erase(it);
    }

    void Graph::remove_property(const std::string &name)
    {
        properties_.remove(name);
    }

    boost::optional<std::reference_wrapper<Property>> Graph::get_property_by_name(const std::string &name)
    {
        return properties_.get_property_by_name(name);
    }

    SocketCollection<InputSocket>& Graph::inputs()
    {
        return inputs_;
    }

    SocketCollection<OutputSocket>& Graph::outputs()
    {
        return outputs_;
    }

    InputSocket& Graph::add_input(const std::string &name, SocketType type)
    {
        return inputs_.add(name, type);
    }

    OutputSocket& Graph::add_output(const std::string &name, const ConnectionDataType &data_type, SocketType type)
    {
        return outputs_.add(name, data_type, type);
    }
}
