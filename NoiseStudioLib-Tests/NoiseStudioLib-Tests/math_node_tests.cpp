#include "catch.h"

#include <nodes/math.h>
#include <nodes/constant_value.h>
#include <graph.h>
#include <graph_executor.h>

using namespace noises;
using namespace noises::nodes;

TEST_CASE("Math node addition", "")
{
    Graph graph;

    ConstantValue& value_1_node = graph.add_node<ConstantValue>();
    ConstantValue& value_2_node = graph.add_node<ConstantValue>();

    value_1_node.set_value_single(1);
    value_2_node.set_value_single(1);

    Math& math_node = graph.add_node<Math>();

    int operation = static_cast<int>(MathOperation::add);
    math_node.property("Operation").set_value<int, 1>(&operation);

    graph.connect(value_1_node.output("Value"), math_node.input("A"));
    graph.connect(value_2_node.output("Value"), math_node.input("B"));

    auto& graph_output_node = graph.add_uniform_output("Output");

    graph.connect(math_node.output("Output"), graph_output_node.input("Input"));

    GraphExecutor executor(graph);
    GraphOutputs outputs = executor.execute();

    int output = outputs.get_uniform<int, 1>("Output");

    REQUIRE(output == 2);
}
