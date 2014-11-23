#ifndef BLANK_GRID_H
#define BLANK_GRID_H

#include "graph_node.h"

namespace noises {
namespace nodes {
    class BlankGrid : public GraphNode
    {
    public:
        BlankGrid();

        std::string node_name() const;

        void execute_uniforms(const CompositeDataBuffer& input, DataBuffer& output) const;

        void set_size(unsigned int width, unsigned int height);
    };
}}



#endif // BLANK_GRID_H
