#ifndef PIXEL_MAPPING_H
#define PIXEL_MAPPING_H

#include "graph_node.h"

namespace noises {
    class OutputSocket;

namespace nodes {
namespace mappings {

    //Maps a grid 1:1, that is, the upper left is 0, 0 and the bottom right is width,height
    class PixelMapping : public GraphNode
    {
    public:
        PixelMapping();

        std::string node_name() const;

        void execute_uniforms(const CompositeDataBuffer& input, DataBuffer& output) const;
        void execute_attributes(const CompositeDataBuffer& input, DataBuffer& output, DataBuffer::size_type index) const;

    private:
        OutputSocket* output_;
    };

} } }

#endif // PIXEL_MAPPING_H
