#include "catch.h"

#include <graph.h>
#include <nodes/constant_value.h>
#include <graph_executor.h>

using namespace noises;
using namespace noises::nodes;

TEST_CASE("Simple graph, can get output", "")
{
    Graph graph;

    ConstantValue& cv = graph.add_node<ConstantValue>();
    cv.set_value_single(7);

    GraphNode& graph_output_node = graph.add_uniform_output("int");

    graph.connect(cv.output("Value"), graph_output_node.input("Input"));

    GraphExecutor e(graph);
    GraphOutputs out(e.execute());

    auto output_value = out.get_uniform<int, 1>("int");
    int value_returned = output_value.value();

    REQUIRE(value_returned == 7);
}

TEST_CASE("Graph has inputs, can get output", "")
{
    Graph graph;

    GraphNode& input = graph.add_uniform_input("Input");
    GraphNode& output = graph.add_uniform_output("Output");

    graph.connect(input.output("Output"), output.input("Input"));

    int value = 6;
    graph.set_input_uniform<int, 1>("Input", &value);

    GraphOutputs out = graph.execute();

    auto output_value = out.get_uniform<int, 1>("Output");
    int value_returned = output_value.value();

    REQUIRE(value_returned == value);
}
