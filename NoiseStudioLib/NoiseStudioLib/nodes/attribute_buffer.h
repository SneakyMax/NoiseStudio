#ifndef ATTRIBUTE_BUFFER_H
#define ATTRIBUTE_BUFFER_H

#include <boost/optional.hpp>

#include "graph_node.h"

namespace noises {
namespace nodes
{
    class AttributeBuffer : public GraphNode
    {
    public:
        AttributeBuffer();

        std::string node_name() const;

        void recalculate_sockets();

        void execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const;

        InputSocket& input();
        const InputSocket& input() const;

        OutputSocket& output();
        const OutputSocket& output() const;

        void set_output_type(const ConnectionDataType& data_type, AttributeInfo attribute_info);

    private:
        InputSocket* input_;
        OutputSocket* output_;

        bool has_manual_output_type_;
    };
} }



#endif // ATTRIBUTE_BUFFER_H
