#include "graph_validator.h"

#include <boost/format.hpp>

#include "graph.h"

namespace noises
{
    GraphValidator::GraphValidator(const Graph& graph) : graph_(graph) { }

    ValidationResults GraphValidator::validate() const
    {
        ValidationResults results;
        verify_all_connections(results);
        verify_no_cycles(results);

        //Cycles will make this not work, so only do it if there aren't any cycles
        if(results)
        {
            verify_all_outputs_satisfied(results);
            verify_all_nodes(results);

            //TODO verify that all attributes connected to a node are the same length
        }

        return results;
    }

    void GraphValidator::verify_all_nodes(ValidationResults& results) const
    {
        for(const GraphNode& node : graph_.nodes())
        {
            node.validate(results);
        }
    }

    void GraphValidator::verify_all_connections(ValidationResults &results) const
    {
        auto connections = graph_.connections();
        auto it = connections.begin();
        for(; it != connections.end(); ++it)
        {
            const Connection& connection = it->get();

            if(!connection.input_output_type_compatible())
            {
                std::string text;
                text += "Incompatible connection. Output type is ";
                text += connection.output().data_type().name_description();
                text += ", and input supports: ";

                for(auto& accepted_data_type : connection.input().accepted_types())
                {
                    text += accepted_data_type.get().name_description() + ", ";
                }

                results.add(text);
            }
        }
    }

    void GraphValidator::verify_no_cycles(ValidationResults &results) const
    {
        //http://en.wikipedia.org/wiki/Topological_sorting#Algorithms
        std::vector<int> sorted_nodes;
        std::vector<int> working_list;
        std::vector<int> edges;

        // Add all nodes without incoming edges
        for(auto& node : graph_.nodes())
        {
            bool has_incoming_edge = false;
            for(auto& socket_ref : node.get().inputs().all_sockets())
            {
                const InputSocket& socket = socket_ref.get();
                if(socket.connection())
                {
                    has_incoming_edge = true;
                    break;
                }
            }
            if(has_incoming_edge == false)
                working_list.push_back(node.get().id());
        }

        // Input buffer nodes are considered automatically to have no incoming edges
        for(auto& node : graph_.input_nodes())
        {
            working_list.push_back(node.get().id());
        }

        // Add all connections as edges
        for(const Connection& connection : graph_.connections())
        {
            edges.push_back(connection.id());
        }

        while(working_list.size() > 0)
        {
            int node_id = working_list.back();
            working_list.pop_back();
            sorted_nodes.push_back(node_id);
            const GraphNode& node = graph_.get_node_by_id(node_id)->get();

            for(const OutputSocket& socket : node.outputs().all_sockets())
            {
                for(const Connection& connection : socket.connections())
                {
                    edges.erase(std::remove(edges.begin(), edges.end(), connection.id()), edges.end());

                    const GraphNode& node_m = *connection.input().parent();
                    bool has_other_incoming_edges = false;

                    for(const InputSocket& m_input : node_m.inputs().all_sockets())
                    {
                        auto connection = m_input.connection();
                        if(connection)
                        {
                            bool connection_has_incoming_edge = std::find(edges.begin(), edges.end(), connection->get().id()) != edges.end();
                            if(connection_has_incoming_edge)
                                has_other_incoming_edges = true;
                        }
                    }

                    if(has_other_incoming_edges == false)
                        working_list.push_back(node_m.id());
                }
            }
        }

        if(edges.size() > 0)
        {
            results.add("Cycle detected in graph. Graph must be an acyclic directed graph.");
        }
    }

    void GraphValidator::verify_all_outputs_satisfied(ValidationResults &results) const
    {
        for(auto node : graph_.output_nodes())
        {
            verify_output_satisfied(node, results);
        }
    }

    void GraphValidator::verify_output_satisfied(const GraphNode &node, ValidationResults &results) const
    {
        auto& inputs = node.inputs();
        for(auto& socket_ref : inputs.all_sockets())
        {
            verify_output_socket_satisfied(node, socket_ref, results);
        }
    }

    void GraphValidator::verify_output_socket_satisfied(const GraphNode &node, const InputSocket& input_socket, ValidationResults &results) const
    {
        auto possible_connection = input_socket.connection();
        if(!possible_connection)
        {
            if(!(node.is_graph_internal_node() && input_socket.optional()))
            {
                std::string text = boost::str(boost::format("Output node %1% is missing an input connection.") % node.display_name());
                results.add(text);
                return;
            }

            if(graph_.get_input_buffer(node.name()))
                return;

            std::string text = boost::str(boost::format("Output node %1% is missing an input connection, or is missing data being set directly on the Graph.") % node.display_name());
            results.add(text);
            return;
        }

        const Connection& connection = possible_connection->get();
        const OutputSocket& output = connection.output();

        if(output.parent() == nullptr)
            throw std::logic_error("Output socket did not have a parent! Internal error!");

        verify_node_satisfied(*output.parent(), results);
    }

    void GraphValidator::verify_node_satisfied(const GraphNode &node, ValidationResults &results) const
    {
        auto& inputs = node.inputs();
        for(auto& socket_ptr : inputs.attribute_sockets())
        {
            verify_node_socket_satisfied(node, socket_ptr, results);
        }
        for(auto& socket_ptr : inputs.uniform_sockets())
        {
            verify_node_socket_satisfied(node, socket_ptr, results);
        }
    }

    void GraphValidator::verify_node_socket_satisfied(const GraphNode &node, const InputSocket& input_socket, ValidationResults &results) const
    {
        auto possible_connection = input_socket.connection();
        if(!possible_connection)
        {
            if(input_socket.optional() == false)
            {
                std::string text = boost::str(boost::format("Node %1% is missing an input connection for %2%, which is not optional.")
                                              % node.display_name() % input_socket.name());
                results.add(text);
            }
        }
        else
        {
            const Connection& connection = possible_connection->get();
            const OutputSocket& output = connection.output();

            if(output.parent() == nullptr)
                throw std::logic_error("Output socket did not have a parent! Internal error!");

            verify_node_satisfied(*output.parent(), results);
        }
    }
}
