#include "catch.h"

#include <graph_validator.h>
#include <graph.h>
#include <nodes/constant_value.h>
#include <nodes/math.h>

using namespace noises;
using namespace noises::nodes;

TEST_CASE("Simple graph validates.", "")
{
    Graph graph;

    std::unique_ptr<nodes::ConstantValue> constant_value(new nodes::ConstantValue);
    int value = 7;
    constant_value->set_value<int, 1>(&value);

    int constant_value_id = graph.add_node(std::move(constant_value));
    graph.add_uniform_output("int");

    graph.connect(graph.get_node_by_id(constant_value_id)->get().outputs()[nodes::ConstantValue::socket_name],
                  graph.get_uniform_output("int")->get().input());

    GraphValidator validator(graph);

    auto validation = validator.validate();
    bool is_valid = validation;

    REQUIRE(is_valid);
}

TEST_CASE("Simple cyclic graph does not validate.", "")
{
    Graph graph;

    nodes::ConstantValue& constant_value = graph.add_node<nodes::ConstantValue>();
    nodes::Math& math_1 = graph.add_node<nodes::Math>();
    nodes::Math& math_2 = graph.add_node<nodes::Math>();

    constant_value.set_value_single(1);

    InputSocket& output_socket = graph.add_uniform_output("Output").input("Input");

    graph.connect(constant_value.output("Value"), math_1.input("A"));
    graph.connect(math_2.output("Output"), math_1.input("B"));

    graph.connect(math_1.output("Output"), math_2.input("A"));
    graph.connect(constant_value.output("Value"), math_2.input("B"));

    graph.connect(math_2.output("Output"), output_socket);

    GraphValidator validator(graph);

    ValidationResults validation = validator.validate();
    bool is_valid = validation;

    REQUIRE_FALSE(is_valid);
}

TEST_CASE("Mismatched socket types do not validate", "")
{
    Graph graph;

    ConstantValue& constant_value = graph.add_node<ConstantValue>();
    GraphNode& output_node = graph.add_attribute_output("Output");

    graph.connect(constant_value, output_node);

    GraphValidator validator(graph);

    ValidationResults validation = validator.validate();
    bool is_valid = validation;

    REQUIRE_FALSE(is_valid);
}
