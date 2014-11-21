#ifndef ADD_H
#define ADD_H

#include <cmath>

#include "graph_node.h"

namespace noises {
namespace nodes {

    enum class MathOperation
    {
        add,
        subtract,
        multiply,
        divide,
        exponent,
        logorithm

        //TODO single-operator math operations (need to remove/hide the B input)
    };

    class Math : public GraphNode
    {
    public:
        Math();

        std::string node_name() const;

        void recalculate_sockets();
        void execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const;
        void validate(ValidationResults& results) const;
    };

} }



#endif // ADD_H
