#include "perlin_noise.h"

#include <array>

namespace noises {
namespace nodes
{

    PerlinNoise::PerlinNoise()
    {
        InputSocket& seed = inputs().add("Seed", SocketType::uniform);
        seed.set_accepts(ConnectionDataType::value<long, 1>());

        // Just a vector2 to start out with
        InputSocket& points = inputs().add("Points", SocketType::attribute);
        points.set_accepts(ConnectionDataType::value<float, 2>());

        outputs().add("Output", ConnectionDataType::value<float, 1>(), SocketType::attribute);
    }
} }
