#include "catch.h"

#include <data_buffer.h>
#include <composite_data_buffer.h>
#include <input_socket.h>
#include <socket_collection.h>

using namespace noises;

TEST_CASE("Can create composite data buffer", "")
{
    CompositeDataBuffer c(4);

    REQUIRE(c.num_uniforms() == 0);
    REQUIRE(c.num_attributes() == 0);
    REQUIRE(c.allocated_uniform_memory() == 0);
}

TEST_CASE("Composite data buffer add/get uniforms")
{
    CompositeDataBuffer c(4);

    DataBuffer d(4);

    OutputSocket output("int", ConnectionDataType::value<int, 3>(), SocketType::Uniform);
    d.add_uniform(ConnectionDataType::value<int, 3>());

    int data[] = { 1, 2, 3 };

    d.set_uniform<int, 3>(output, data);
    c.add_uniform(ConnectionDataType::value<int, 3>(), d.get_uniform_block(output.index()));

    SECTION("Can add uniform")
    {
        REQUIRE(c.num_uniforms() == 1);
        REQUIRE(c.num_attributes() == 0);
        REQUIRE(c.allocated_uniform_memory() == sizeof(int) * 3);
    }

    SECTION("Can retrieve uniform")
    {
        InputSocket input("int", SocketType::Uniform);
        input.set_accepts(ConnectionDataType::value<int, 3>());

        const int* data_ret = c.get_uniform<int, 3>(input);

        REQUIRE(data_ret[0] == 1);
        REQUIRE(data_ret[1] == 2);
        REQUIRE(data_ret[2] == 3);
    }
}

TEST_CASE("Composite data buffer add/get attribute", "")
{
    int n = 4;
    CompositeDataBuffer c(n);
    DataBuffer d(n);

    OutputSocket output("int", ConnectionDataType::value<int, 3>(), SocketType::Attribute);
    d.add_attribute(ConnectionDataType::value<int, 3>());

    for(int i = 0; i < n; i++)
    {
        int data[] = { i, i * 10, i * 100 };
        d.set_attribute<int, 3>(output, i, data);
    }

    c.add_attribute(ConnectionDataType::value<int, 3>(), d.get_memory_block(0));

    SECTION("Can add attribute")
    {
        REQUIRE(c.num_attributes() == 1);
        REQUIRE(c.num_uniforms() == 0);
        REQUIRE(c.attribute_size() == n);

        auto& data_type = ConnectionDataType::value<int, 3>();
        REQUIRE(c.get_attribute_type(0) == data_type);
    }

    SECTION("Can get attribute data")
    {
        InputSocket input("int", SocketType::Attribute);
        input.set_accepts(ConnectionDataType::value<int, 3>());

        for(int i = 0; i < n; i++)
        {
            const int* data_ret = c.get_attribute<int, 3>(input, i);

            REQUIRE(data_ret[0] == i);
            REQUIRE(data_ret[1] == i * 10);
            REQUIRE(data_ret[2] == i * 100);
        }
    }
}

TEST_CASE("Multiple attributes flip-flop")
{
    SocketCollection<OutputSocket> outputs;
    SocketCollection<InputSocket> inputs;

    outputs.add(std::unique_ptr<OutputSocket>(
        new OutputSocket("a", ConnectionDataType::value<int, 2>(), SocketType::Attribute)));

    outputs.add(std::unique_ptr<OutputSocket>(
        new OutputSocket("b", ConnectionDataType::value<float, 2>(), SocketType::Attribute)));

    // Inputs are added b, a instead of a, b
    inputs.add(std::unique_ptr<InputSocket>(
        new InputSocket("b", SocketType::Attribute)));

    inputs.add(std::unique_ptr<InputSocket>(
        new InputSocket("a", SocketType::Attribute)));

    inputs["b"].set_accepts(ConnectionDataType::value<float, 2>());
    inputs["a"].set_accepts(ConnectionDataType::value<int, 2>());

    int n = 4;
    DataBuffer d(n);
    CompositeDataBuffer c(n);

    d.add(outputs);

    for(int i = 0; i < n; i++)
    {
        int int_data[] = { i, i * 10 };
        float float_data[] = { i + 0.5f, (i * 10) + 0.5f };

        d.set_attribute<int, 2>(outputs["a"], i, int_data);
        d.set_attribute<float, 2>(outputs["b"], i, float_data);
    }

    c.add_attribute(ConnectionDataType::value<float, 2>(), d.get_memory_block(1));
    c.add_attribute(ConnectionDataType::value<int, 2>(), d.get_memory_block(0));

    for(int i = 0; i < n; i++)
    {
        const float* float_data_ret = c.get_attribute<float, 2>(inputs["b"], i);
        const int* int_data_ret = c.get_attribute<int, 2>(inputs["a"], i);

        REQUIRE(float_data_ret[0] == i + 0.5f);
        REQUIRE(float_data_ret[1] == (i * 10) + 0.5f);

        REQUIRE(int_data_ret[0] == i);
        REQUIRE(int_data_ret[1] == i * 10);
    }
}
