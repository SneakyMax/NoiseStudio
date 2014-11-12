#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>

#include "graph_node.h"

namespace noises
{
    class Graph
    {
    public:
        Graph();

        /** Adds a graph node to the graph. Returns the id of the added node. **/
        int add(std::unique_ptr<GraphNode> node);

        /** Removes a graph node from the graph. The passed in reference is invalidated (the node is destroyed) **/
        void remove(GraphNode* node);


        /** Gets a list of graph nodes by their node name (same among node types) **/
        std::vector<GraphNode*> get_nodes_by_node_name(const std::string& name) const;

        /** Gets a graph node by its unique name. **/
        GraphNode* get_node_by_name(const std::string& name) const;

        /** Gets a graph node by its id. **/
        GraphNode* get_node_by_id(int id) const;


        /** Connects the output sockets of one graph node to the input sockets of another graph node. **/
        void connect(OutputSocket& output, InputSocket& input);

        /** Disconnects the connection for an input socket. **/
        void disconnect(InputSocket& input);

        /** Disconnects all connections coming out of an output socket. **/
        void disconnect_all(OutputSocket& output);

    private:
        void remove_connection(const Connection* connection);

        int id_counter_;

        std::vector<std::unique_ptr<GraphNode>> nodes_;
        std::vector<std::unique_ptr<Connection>> connections_;
    };
}



#endif // GRAPH_H
