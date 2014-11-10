#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <vector>
#include <functional>
#include "socket_collection.h"
#include "input_socket.h"
#include "output_socket.h"
#include "data_buffer.h"

namespace noises
{
    class GraphNode
    {
    public:
        GraphNode();
        virtual ~GraphNode();

        virtual const SocketCollection<InputSocket>& inputs() const { return inputs_; }
        virtual const SocketCollection<OutputSocket>& outputs() const { return outputs_; }

    protected:
        SocketCollection<InputSocket> inputs_;
        SocketCollection<OutputSocket> outputs_;
    };
}



#endif // GRAPH_NODE_H
