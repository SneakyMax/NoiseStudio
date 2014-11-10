#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include "graph_node.h"
#include <memory>
#include "input_socket.h"
#include "output_socket.h"

namespace noises {
namespace nodes
{
    class PerlinNoise : public GraphNode
    {
    public:
        PerlinNoise();

    };
} }

#endif // PERLIN_NOISE_H
