#ifndef UNIT_SQUARE_MAPPING_H
#define UNIT_SQUARE_MAPPING_H

#include "graph_node.h"

namespace noises {
    class OutputSocket;

namespace nodes {
namespace mappings {

    // Maps a grid to the unit square. (-1, -1) to (1, 1)
    class UnitSquareMapping : public GraphNode
    {
    public:
        UnitSquareMapping();

        std::string node_name() const;

        void execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const;
        void execute_attributes(const CompositeDataBuffer &input, DataBuffer &output, DataBuffer::size_type index) const;

    private:
        OutputSocket* output_;
    };

}}}



#endif // UNIT_SQUARE_MAPPING_H
