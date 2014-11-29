#include "catch.h"

#include <nodes/perlin_noise.h>
#include <graph.h>
#include <nodes/blank_grid.h>
#include <nodes/constant_value.h>
#include <nodes/mappings/unit_square_mapping.h>
#include <graph_outputs.h>
#include <nodes/type_conversion.h>

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
    Graph graph;

    BlankGrid& grid = graph.add_node<BlankGrid>();
    grid.set_size(10, 10);

    UnitSquareMapping& mapping = graph.add_node<UnitSquareMapping>();
    TypeConversion& mapping_to_noise = graph.add_node<TypeConversion>();
    mapping_to_noise.set_type(TypeConversionTargetType::float_t);

    ConstantValue& seed = graph.add_node<ConstantValue>();
    seed.set_value_single(5l);

    PerlinNoise& noise = graph.add_node<PerlinNoise>();

    GraphNode& output_node = graph.add_attribute_output("Output");

    graph.connect(grid, mapping);

    graph.connect(mapping, mapping_to_noise);

    graph.connect(seed, noise, "Seed");
    graph.connect(mapping_to_noise, noise, "Points");

    graph.connect(noise, output_node);

    GraphOutputs outputs = graph.execute();

    std::vector<float> out = outputs.get_attribute_all_vector<float>("Output");

    REQUIRE(out.size() == 100);

    bool has_all_zeros = true;
    for(std::size_t i = 0; i < out.size(); i++)
    {
        REQUIRE(out[i] <= 1);
        REQUIRE(out[i] >= -1);

        if(out[i] != 0)
            has_all_zeros = false;
    }

    REQUIRE_FALSE(has_all_zeros);
}
