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

TEST_CASE("Math node valid with same data type, one 1 dimensional and one 3 dimensional", "")
{
    Graph graph;

    ConstantValue& value_1_node = graph.add_node<ConstantValue>();
    ConstantValue& value_2_node = graph.add_node<ConstantValue>();

    value_1_node.set_value_single(5);

    std::array<int, 3> value_2_value { 1, 2, 3 };
    value_2_node.set_value(ptr_array<int, 3>(value_2_value));

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

    REQUIRE(is_valid);
}

TEST_CASE("Add scalar and vector", "")
{
    Graph graph;

    ConstantValue& value_1_node = graph.add_node<ConstantValue>();
    ConstantValue& value_2_node = graph.add_node<ConstantValue>();

    value_1_node.set_value_single(5);

    std::array<int, 3> value_2_value { 1, 2, 3 };
    value_2_node.set_value(ptr_array<int, 3>(value_2_value));

    Math& math_node = graph.add_node<Math>();

    int operation = static_cast<int>(MathOperation::add);
    math_node.property("Operation").set_value<int, 1>(&operation);

    graph.connect(value_1_node.output("Value"), math_node.input("A"));
    graph.connect(value_2_node.output("Value"), math_node.input("B"));

    auto& graph_output_node = graph.add_uniform_output("Output");

    graph.connect(math_node.output("Output"), graph_output_node.input("Input"));

    GraphOutputs outputs = graph.execute();

    const ptr_array<int, 3> out_value = outputs.get_uniform<int, 3>("Output");

    REQUIRE(out_value[0] == 6);
    REQUIRE(out_value[1] == 7);
    REQUIRE(out_value[2] == 8);
}

TEST_CASE("Subtract two vectors", "")
{
    Graph graph;

    ConstantValue& value_1_node = graph.add_node<ConstantValue>();
    ConstantValue& value_2_node = graph.add_node<ConstantValue>();

    std::array<float, 3> value_1_value { 1.5, 2.5, 3.5 };
    value_1_node.set_value(ptr_array<float, 3>(value_1_value));

    std::array<float, 3> value_2_value { 3.5, 2.5, 1.5 };
    value_2_node.set_value(ptr_array<float, 3>(value_2_value));

    Math& math_node = graph.add_node<Math>();

    int operation = static_cast<int>(MathOperation::subtract);
    math_node.property("Operation").set_value<int, 1>(&operation);

    graph.connect(value_1_node.output("Value"), math_node.input("A"));
    graph.connect(value_2_node.output("Value"), math_node.input("B"));

    auto& graph_output_node = graph.add_uniform_output("Output");

    graph.connect(math_node.output("Output"), graph_output_node.input("Input"));

    GraphOutputs outputs = graph.execute();

    const ptr_array<float, 3> out_value = outputs.get_uniform<float, 3>("Output");

    REQUIRE(out_value[0] == -2);
    REQUIRE(out_value[1] == 0);
    REQUIRE(out_value[2] == 2);
}

TEST_CASE("Multiply a scalar array with a vector array", "")
{
    Graph graph;

    Math& math_node = graph.add_node<Math>();

    int operation = static_cast<int>(MathOperation::multiply);
    math_node.property("Operation").set_value<int, 1>(&operation);

    GraphNode& a_input = graph.add_attribute_input("A");
    GraphNode& b_input = graph.add_attribute_input("B");

    GraphNode& graph_output = graph.add_attribute_output("Output");

    graph.connect(a_input.output("Output"), math_node.input("A"));
    graph.connect(b_input.output("Output"), math_node.input("B"));

    graph.connect(math_node.output("Output"), graph_output.input("Input"));

    std::vector<int> a_values;
    std::vector<std::array<int, 3>> b_values;

    a_values.push_back(2);
    b_values.push_back(std::array<int, 3> { 3, 4, 5 });

    a_values.push_back(3);
    b_values.push_back(std::array<int, 3> { 4, 5, 6 });

    a_values.push_back(4);
    b_values.push_back(std::array<int, 3> { 5, 6, 7 });

    graph.set_input_attribute<int, 1>("A", &a_values[0], 3);
    graph.set_input_attribute<int, 3>("B", &b_values[0][0], 3);

    GraphOutputs outputs = graph.execute();

    auto out_1 = outputs.get_attribute<int, 3>("Output", 0);
    auto out_2 = outputs.get_attribute<int, 3>("Output", 1);
    auto out_3 = outputs.get_attribute<int, 3>("Output", 2);

    REQUIRE(out_1[0] == 6);
    REQUIRE(out_1[1] == 8);
    REQUIRE(out_1[2] == 10);

    REQUIRE(out_2[0] == 12);
    REQUIRE(out_2[1] == 15);
    REQUIRE(out_2[2] == 18);

    REQUIRE(out_3[0] == 20);
    REQUIRE(out_3[1] == 24);
    REQUIRE(out_3[2] == 28);
}
