#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

#include "connection_data_type.h"

namespace noises
{
    class InputSocket;
    class OutputSocket;
    class Graph;

    class Connection
    {
    public:
        Connection(InputSocket& input, OutputSocket& output, Graph& parent_graph);

        bool input_output_type_compatible() const;

        const InputSocket& input() const;
        const OutputSocket& output() const;
        const Graph& parent_graph() const;

        const ConnectionDataType& data_type() const;

        int id() const;
        void set_id(int id);

        void disconnect();
    private:

        // The input is generally the *right* side of a connection
        InputSocket& input_;

        // The output is generally the *left* side of a connection
        OutputSocket& output_;

        Graph& parent_graph_;

        int id_;
    };
}

#endif // CONNECTION_H
