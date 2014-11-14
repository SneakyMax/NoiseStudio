#include "graph_executor.h"

namespace noises
{
    GraphExecutor::GraphExecutor(Graph &graph) : graph_(graph)
    {

    }

    Graph& GraphExecutor::graph()
    {
        return graph_;
    }
}
