#include "catch.h"

#include <nodes/perlin_noise.h>

using namespace noises;
using namespace noises::nodes;

TEST_CASE("Getting permutations of hypercube", "")
{
    std::vector<std::array<signed char, 2>> perm_2 = HypercubeEdges<2>::edge_vectors();
    REQUIRE(perm_2.size() == 4);

    std::vector<std::array<signed char, 3>> perm_3 = HypercubeEdges<3>::edge_vectors();
    REQUIRE(perm_3.size() == 12);

    std::vector<std::array<signed char, 4>> perm_4 = HypercubeEdges<4>::edge_vectors();
    REQUIRE(perm_4.size() == 32);

    std::vector<std::array<signed char, 5>> perm_5 = HypercubeEdges<5>::edge_vectors();
    REQUIRE(perm_5.size() == 80);
}
