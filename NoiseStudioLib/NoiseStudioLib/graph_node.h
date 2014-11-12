#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <vector>
#include <functional>
#include "socket_collection.h"
#include "data_buffer.h"
#include "input_socket.h"
#include "output_socket.h"

namespace noises
{
    class CompositeDataBuffer;

    class GraphNode
    {
    public:
        GraphNode();
        virtual ~GraphNode();

        /** Gets the node name which is the same for all nodes of the same type. E.g. "PerlinNoise" **/
        virtual std::string node_name() const = 0;

        /** Gets the name which   **/
        const std::string& name() const;
        void set_name(const std::string& name);

        SocketCollection<InputSocket>& inputs();
        SocketCollection<OutputSocket>& outputs();

        virtual const SocketCollection<InputSocket>& inputs() const;
        virtual const SocketCollection<OutputSocket>& outputs() const;

        /** Executes a graph node and computes its uniform outputs. This method is executed once. **/
        virtual void execute_uniforms(const CompositeDataBuffer& input, DataBuffer& output) const;

        /** Executes a graph node and computes its attribute outputs. This method is called once for each index in the final output (e.g. x/y) **/
        virtual void execute_attributes(const CompositeDataBuffer& input, DataBuffer& output, DataBuffer::size_type index) const;

        int id() const;
        void set_id(int id);

    protected:
        SocketCollection<InputSocket> inputs_;
        SocketCollection<OutputSocket> outputs_;

        std::string name_;
        int id_;
    };
}



#endif // GRAPH_NODE_H
