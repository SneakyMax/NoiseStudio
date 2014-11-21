#ifndef GRAPH_VALIDATOR_H
#define GRAPH_VALIDATOR_H

#include "validation_results.h"

namespace noises
{
    class Graph;
    class GraphNode;
    class InputSocket;

    class GraphValidator
    {
    public:
        GraphValidator(const Graph& graph);

        ValidationResults validate() const;

    private:
        void verify_all_connections(ValidationResults& results) const;

        void verify_no_cycles(ValidationResults& results) const;

        void verify_all_outputs_satisfied(ValidationResults& results) const;
        void verify_output_satisfied(const GraphNode& node, ValidationResults& results) const;
        void verify_output_socket_satisfied(const GraphNode& node, const InputSocket& input_socket, ValidationResults& results) const;

        void verify_node_satisfied(const GraphNode& node, ValidationResults& results) const;
        void verify_node_socket_satisfied(const GraphNode& node, const InputSocket& input_socket, ValidationResults& results) const;

        void verify_all_nodes(ValidationResults& results) const;

        const Graph& graph_;
    };
}



#endif // GRAPH_VALIDATOR_H
