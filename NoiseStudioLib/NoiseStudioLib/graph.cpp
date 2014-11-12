#include "graph.h"

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

        // Remove the node from the list
        auto it = nodes_.begin();
        for(; it != nodes_.end(); ++it)
        {
            if(it->get() == node)
                break;
        }
        if(it != nodes_.end())
            nodes_.erase(it);
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

    GraphNode* Graph::get_node_by_name(const std::string &name) const
    {
        for(auto& node : nodes_)
        {
            if(node->name() == name)
                return node.get();
        }
        return nullptr;
    }

    GraphNode* Graph::get_node_by_id(int id) const
    {
        for(auto& node : nodes_)
        {
            if(node->id() == id)
                return node.get();
        }
        return nullptr;
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
}
