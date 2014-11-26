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

    template<unsigned int N>
    class HypercubeEdges
    {
    public:
        static std::vector<std::array<signed char, N>> edge_vectors();
        static std::vector<std::array<signed char, N>> get_permutations_no_zero();
    };

    template<unsigned int N, unsigned int NCurrent>
    class HypercubeEdges_Permutations
    {
    public:
        static void add_permutations(std::array<signed char, N> a,  std::vector<std::array<signed char, N>>& out)
        {
            a[N - NCurrent] = -1;
            HypercubeEdges_Permutations<N, NCurrent - 1>::add_permutations(a, out);

            a[N - NCurrent] = 1;
            HypercubeEdges_Permutations<N, NCurrent - 1>::add_permutations(a, out);
        }
    };

    template<unsigned int N>
    class HypercubeEdges_Permutations<N, 0>
    {
    public:
        static void add_permutations(std::array<signed char, N> a, std::vector<std::array<signed char, N>>& out)
        {
            out.push_back(a);
        }
    };

    /** Gets vectors which point to every edge of an N-Dimensional hypercube.  */
    template<unsigned int N>
    std::vector<std::array<signed char, N>> HypercubeEdges<N>::edge_vectors()
    {
        std::vector<std::array<signed char, N>> out;

        std::vector<std::array<signed char, N - 1>> without_zero = HypercubeEdges<N - 1>::get_permutations_no_zero();

        // Put the 0 at each position in the array
        for(int i = 0; i < N; i++)
        {
            // Using all permutations without the zero
            for(int j = 0; j < without_zero.size(); j++)
            {
                std::array<signed char, N - 1>& without = without_zero[j];
                std::array<signed char, N> a;
                a[i] = 0;

                for(int q = 0, p = 0; q < N - 1; q++, p++)
                {
                    if(q == i)
                        p++;
                    a[p] = without[q];
                }

                out.push_back(a);
            }
        }

        return out;
    }

    template<unsigned int N>
    std::vector<std::array<signed char, N>> HypercubeEdges<N>::get_permutations_no_zero()
    {
        std::vector<std::array<signed char, N>> out;
        HypercubeEdges_Permutations<N, N>::add_permutations(std::array<signed char, N>(), out);
        return out;
    }
} }

#endif // PERLIN_NOISE_H
