#ifndef BUFFER_H
#define BUFFER_H

#include <boost/optional.hpp>

#include "graph_node.h"

namespace noises {
namespace nodes
{
    class UniformBuffer : public GraphNode
    {
    public:
        UniformBuffer();

        std::string node_name() const;

        void recalculate_sockets();

        void execute_uniforms(const CompositeDataBuffer& input, DataBuffer& output) const;

        InputSocket& input();
        OutputSocket& output();

        const InputSocket& input() const;
        const OutputSocket& output() const;

        void set_output_type(const ConnectionDataType& data_type);

    private:
        InputSocket* input_;
        OutputSocket* output_;

        bool has_manual_output_type_;
    };
} }



#endif // BUFFER_H
