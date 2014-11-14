#ifndef GRAPH_EXECUTOR_H
#define GRAPH_EXECUTOR_H

namespace noises
{
    class Graph;

    class GraphExecutor
    {
    public:
        GraphExecutor(Graph& graph);

        Graph& graph();

    private:
        Graph& graph_;
    };
}



#endif // GRAPH_EXECUTOR_H
