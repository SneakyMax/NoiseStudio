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

    private:
        InputSocket* input_;
        OutputSocket* output_;
    };
} }



#endif // ATTRIBUTE_BUFFER_H
