#ifndef GRAPH_EXECUTOR_H
#define GRAPH_EXECUTOR_H

#include "graph_outputs.h"
#include "validation_results.h"
#include "data_buffer.h"

#include <deque>

namespace noises
{
    class Graph;
    class GraphNode;

    class GraphExecutor
    {
    public:
        GraphExecutor(const Graph& graph);

        const Graph& graph();

        ValidationResults validate_graph() const;
        GraphOutputs execute();

    private:
        const Graph& graph_;

        void get_topological_order();
        GraphOutputs execute_internal();
        void execute_node(int node_id);
        std::vector<int> get_node_dependencies(const GraphNode& node);
        DataBuffer& get_buffer(int node_id, std::size_t buffer_attribute_length);
        std::size_t get_attribute_length(const GraphNode& node);
        std::unordered_map<int, std::reference_wrapper<DataBuffer>> get_node_dependency_buffers(const GraphNode& node);
        std::unique_ptr<DataBuffer> extract_buffer(const DataBuffer& buffer);

        std::deque<std::vector<int>> topological_order_;
        std::deque<std::vector<std::pair<int, std::unique_ptr<DataBuffer>>>> buffer_stack_;
    };
}



#endif // GRAPH_EXECUTOR_H
