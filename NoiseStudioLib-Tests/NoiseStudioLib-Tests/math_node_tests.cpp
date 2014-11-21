#include "catch.h"

#include <nodes/math.h>
#include <nodes/constant_value.h>
#include <graph.h>
#include <graph_executor.h>
#include <graph_validator.h>

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

TEST_CASE("Math node multiplication", "")
{
    Graph graph;

    ConstantValue& value_1_node = graph.add_node<ConstantValue>();
    ConstantValue& value_2_node = graph.add_node<ConstantValue>();

    value_1_node.set_value_single(2);
    value_2_node.set_value_single(3);

    Math& math_node = graph.add_node<Math>();

    int operation = static_cast<int>(MathOperation::multiply);
    math_node.property("Operation").set_value<int, 1>(&operation);

    graph.connect(value_1_node.output("Value"), math_node.input("A"));
    graph.connect(value_2_node.output("Value"), math_node.input("B"));

    auto& graph_output_node = graph.add_uniform_output("Output");

    graph.connect(math_node.output("Output"), graph_output_node.input("Input"));

    GraphExecutor executor(graph);
    GraphOutputs outputs = executor.execute();

    int output = outputs.get_uniform<int, 1>("Output");

    REQUIRE(output == 6);
}

TEST_CASE("Division divide by 0 outputs 0", "")
{
    Graph graph;

    ConstantValue& value_1_node = graph.add_node<ConstantValue>();
    ConstantValue& value_2_node = graph.add_node<ConstantValue>();

    value_1_node.set_value_single(5);
    value_2_node.set_value_single(0);

    Math& math_node = graph.add_node<Math>();

    int operation = static_cast<int>(MathOperation::divide);
    math_node.property("Operation").set_value<int, 1>(&operation);

    graph.connect(value_1_node.output("Value"), math_node.input("A"));
    graph.connect(value_2_node.output("Value"), math_node.input("B"));

    auto& graph_output_node = graph.add_uniform_output("Output");

    graph.connect(math_node.output("Output"), graph_output_node.input("Input"));

    GraphExecutor executor(graph);
    GraphOutputs outputs = executor.execute();

    int output = outputs.get_uniform<int, 1>("Output");

    REQUIRE(output == 0);
}

TEST_CASE("Math node invalid with different data types", "")
{
    Graph graph;

    ConstantValue& value_1_node = graph.add_node<ConstantValue>();
    ConstantValue& value_2_node = graph.add_node<ConstantValue>();

    value_1_node.set_value_single(5);
    value_2_node.set_value_single(5.5f);

    Math& math_node = graph.add_node<Math>();

    int operation = static_cast<int>(MathOperation::add);
    math_node.property("Operation").set_value<int, 1>(&operation);

    graph.connect(value_1_node.output("Value"), math_node.input("A"));
    graph.connect(value_2_node.output("Value"), math_node.input("B"));

    auto& graph_output_node = graph.add_uniform_output("Output");

    graph.connect(math_node.output("Output"), graph_output_node.input("Input"));

    GraphValidator validator(graph);
    ValidationResults results = validator.validate();
    bool is_valid = results;

    REQUIRE(is_valid == false);
}
