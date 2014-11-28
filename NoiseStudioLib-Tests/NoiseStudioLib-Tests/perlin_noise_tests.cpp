#include "catch.h"

#include <nodes/perlin_noise.h>
#include <graph.h>
#include <nodes/blank_grid.h>
#include <nodes/constant_value.h>
#include <nodes/mappings/pixel_mapping.h>
#include <graph_outputs.h>

using namespace noises;
using namespace noises::nodes;
using namespace noises::nodes::mappings;

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

TEST_CASE("Basic 2-dimensional noise", "")
{
    return;
    Graph graph;

    BlankGrid& grid = graph.add_node<BlankGrid>();
    grid.set_size(10, 10);

    PixelMapping& mapping = graph.add_node<PixelMapping>();

    ConstantValue& seed = graph.add_node<ConstantValue>();
    seed.set_value_single(5);

    PerlinNoise& noise = graph.add_node<PerlinNoise>();

    GraphNode& output_node = graph.add_attribute_output("Output");

    graph.connect(seed.output("Value"), noise.input("Seed"));
    graph.connect(grid.output("Grid"), mapping.input("Grid"));
    graph.connect(mapping.output("Mapped"), noise.input("Points"));
    graph.connect(noise.output("Output"), output_node.input("Input"));

    GraphOutputs outputs = graph.execute();
}
