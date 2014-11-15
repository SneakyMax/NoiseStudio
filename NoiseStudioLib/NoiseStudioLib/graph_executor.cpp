#include "graph_executor.h"

#include <boost/format.hpp>

#include "graph.h"
#include "graph_node.h"
#include "graph_validator.h"

namespace noises
{
    GraphExecutor::GraphExecutor(const Graph &graph) : graph_(graph)
    {

    }

    const Graph& GraphExecutor::graph()
    {
        return graph_;
    }

    GraphOutputs&& GraphExecutor::execute() const
    {
        if(!validate_graph())
            throw std::logic_error("Could not execute graph. It is invalid.");

        auto topological_order = get_topological_order();

        return std::move(GraphOutputs());

        //TODO
    }

    ValidationResults GraphExecutor::validate_graph() const
    {
        GraphValidator validator(graph_);
        return validator.validate();
    }

    std::stack<std::vector<int>> GraphExecutor::get_topological_order() const
    {
        std::stack<std::vector<int>> out;

        std::vector<int> root;
        for(const GraphNode& output_node : graph_.output_nodes())
        {
            root.push_back(output_node.id());
        }
        out.push(root);

        // Won't run infinitely because the graph has been validated as non-cyclic
        while(true)
        {
            std::vector<int> next_stack_layer;

            for(int node_id : out.top())
            {
                const GraphNode& node = *graph_.get_node_by_id(node_id);

                for(const InputSocket& socket : node.inputs().all_sockets())
                {
                    auto possible_connection = socket.connection();
                    if(possible_connection)
                    {
                        const Connection& connection = *possible_connection;
                        const GraphNode* connection_output = connection.output().parent();
                        next_stack_layer.push_back(connection_output->id());
                    }
                }
            }

            if(next_stack_layer.size() > 0)
            {
                out.push(next_stack_layer);
            }
            else break;
        }

        return out;
    }
}
