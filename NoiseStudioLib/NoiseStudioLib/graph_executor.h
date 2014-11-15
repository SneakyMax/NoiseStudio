#ifndef GRAPH_EXECUTOR_H
#define GRAPH_EXECUTOR_H

#include "graph_outputs.h"
#include "validation_results.h"

#include <stack>

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
        GraphOutputs&& execute() const;

    private:
        const Graph& graph_;

        std::stack<std::vector<int>> get_topological_order() const;
    };
}



#endif // GRAPH_EXECUTOR_H
