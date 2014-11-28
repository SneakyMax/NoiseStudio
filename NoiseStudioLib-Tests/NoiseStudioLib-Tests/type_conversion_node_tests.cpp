#include "catch.h"

#include <graph.h>
#include <graph_outputs.h>
#include <nodes/type_conversion.h>
#include <nodes/constant_value.h>

using namespace noises;
using namespace noises::nodes;

TEST_CASE("Can convert int to long", "")
{
    Graph graph;

    ConstantValue& value = graph.add_node<ConstantValue>();

    TypeConversion& conversion = graph.add_node<TypeConversion>();

    GraphNode& output = graph.add_uniform_output("Output");

    graph.connect(value, conversion);
    graph.connect(conversion, output);

    value.set_value_single(static_cast<int>(5));
    conversion.set_type(TypeConversionTargetType::long_t);

    GraphOutputs outputs = graph.execute();

    long result = outputs.get_uniform<long, 1>("Output").value();

    REQUIRE(result == 5l);
}

TEST_CASE("Can convert float to int", "")
{
    Graph graph;

    ConstantValue& value = graph.add_node<ConstantValue>();

    TypeConversion& conversion = graph.add_node<TypeConversion>();

    GraphNode& output = graph.add_uniform_output("Output");

    graph.connect(value, conversion);
    graph.connect(conversion, output);

    value.set_value_single(static_cast<float>(5.5f));
    conversion.set_type(TypeConversionTargetType::int_t);

    GraphOutputs outputs = graph.execute();

    long result = outputs.get_uniform<int, 1>("Output").value();

    REQUIRE(result == 5);
}

TEST_CASE("Convert multidimensional double to long","")
{
    Graph graph;

    ConstantValue& value = graph.add_node<ConstantValue>();

    TypeConversion& conversion = graph.add_node<TypeConversion>();

    GraphNode& output = graph.add_uniform_output("Output");

    graph.connect(value, conversion);
    graph.connect(conversion, output);

    double value_val[] { 5.5, 2.1, 9 };
    value.set_value<double, 3>(value_val);

    conversion.set_type(TypeConversionTargetType::long_t);

    GraphOutputs outputs = graph.execute();

    const ptr_array<long, 3> result = outputs.get_uniform<long, 3>("Output");

    REQUIRE(result[0] == 5);
    REQUIRE(result[1] == 2);
    REQUIRE(result[2] == 9);
}

TEST_CASE("Convert attribute multidimensional long to float", "")
{
    return;
    int length = 5;
    Graph graph;

    TypeConversion& conversion = graph.add_node<TypeConversion>();

    GraphNode& output = graph.add_attribute_output("Output");

    graph.connect(conversion, output);

    std::vector<std::array<long, 2>> input_values;
    for(int i = 0; i < length; i++)
    {
        input_values.push_back(std::array<long, 2> { i, i * 7 });
    }

    GraphNode& input_node = graph.add_attribute_input("Input");
    graph.set_input_attribute<long, 2>("Input", &input_values[0][0], length);

    graph.connect(input_node, conversion);

    conversion.set_type(TypeConversionTargetType::float_t);

    GraphOutputs outputs = graph.execute();

    for(int i = 0; i < length; i++)
    {
        const ptr_array<float, 2> result = outputs.get_attribute<float, 2>("Input", i);
        REQUIRE(result[0] == i);
        REQUIRE(result[1] == i * 7);
    }
}
