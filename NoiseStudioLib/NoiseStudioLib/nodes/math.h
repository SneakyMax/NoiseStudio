#ifndef ADD_H
#define ADD_H

#include <cmath>

#include "graph_node.h"

namespace noises {
namespace nodes {

    enum class MathOperation
    {
        //binary
        add,
        subtract,
        multiply,
        divide,
        exponent,
        logorithm,
        mod,

        //binary vectors
        dot,
        cross,

        //unary
        negate,
        square_root,
        cube_root,
        log_e,
        log_10,
        log_2,
        ceiling,
        floor,
        round,

        //unary trig
        sin,
        cos,
        tan,
        asin,
        acos,
        atan,

        //binary trig
        atan2
    };

    class Math : public GraphNode
    {
    public:
        Math();

        std::string node_name() const;

        void recalculate_sockets();
        void execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const;
        void execute_attributes(const CompositeDataBuffer &input, DataBuffer &output, DataBuffer::size_type index) const;

        void validate(ValidationResults& results) const;
    };

} }



#endif // ADD_H
