#include "catch.h"

#include <graph.h>
#include <nodes/mappings/pixel_mapping.h>
#include <nodes/blank_grid.h>
#include <nodes/constant_value.h>
#include <graph_outputs.h>

using namespace noises;
using namespace noises::nodes;
using namespace noises::nodes::mappings;

TEST_CASE("Can make a blank grid.", "")
{
    Graph graph;

    BlankGrid& grid = graph.add_node<BlankGrid>();
    grid.set_size(10, 10);

    GraphNode& output_node = graph.add_attribute_output("Output");

    graph.connect(grid.output("Grid"), output_node.input("Input"));

    GraphOutputs outputs = graph.execute();

    int attribute_length = outputs.attribute_info("Output").length();

    REQUIRE(attribute_length == 100);
}
