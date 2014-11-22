#include "catch.h"

#include <memory>
#include <data_buffer.h>
#include <connection_data_type.h>
#include <socket_collection.h>

using namespace noises;

TEST_CASE("Testing DataBuffer", "")
{
    SECTION("Can create data buffer")
    {
        DataBuffer d(4);

        REQUIRE(d.attribute_size() == 4);
    }

    SECTION("Can add int 1-dimensional data type to data buffer")
    {
        DataBuffer d(4);
        d.add_attribute(ConnectionDataType::value<int, 1>());

        REQUIRE(d.attribute_size() == 4);
        REQUIRE(d.num_attributes() == 1);
        REQUIRE(d.allocated_attribute_memory() == sizeof(int) * 4);

        auto& type = ConnectionDataType::value<int, 1>();
        REQUIRE(d.get_attribute_type(0) == type);
    }

    SECTION("Can set values of 1-dimensional int array - attribute")
    {
        int n = 4;
        DataBuffer d(n);
        OutputSocket socket("int", ConnectionDataType::value<int, 1>(), SocketType::attribute);

        d.add_attribute(ConnectionDataType::value<int, 1>());

        for(int i = 0; i < n; i++)
        {
            int value = i;
            d.set_attribute<int, 1>(socket, i, &value);
        }
    }

    SECTION("Can get and set values of 1-dimensional int array - attribute")
    {
        int n = 4;
        DataBuffer d(n);
        OutputSocket socket("int", ConnectionDataType::value<int, 1>(), SocketType::attribute);

        d.add_attribute(ConnectionDataType::value<int, 1>());

        for(int i = 0; i < n; i++)
        {
            int value = i;
            d.set_attribute<int, 1>(socket, i, &value);
        }

        for(int i = 0; i < n; i++)
        {
            auto value = d.get_attribute<int, 1>(socket, i);
            REQUIRE(*value == i);
        }
    }

    SECTION("Can get and set values of a 3-dimensional int array")
    {
        int n = 4;
        DataBuffer d(n);
        OutputSocket socket("int", ConnectionDataType::value<int, 3>(), SocketType::attribute);

        d.add_attribute(ConnectionDataType::value<int, 3>());

        for(int i = 0; i < n; i++)
        {
            int values[] = { i, i * 10, i * 100 };
            d.set_attribute<int, 3>(socket, i, values);
        }

        for(int i = 0; i < n; i++)
        {
            auto value = d.get_attribute<int, 3>(socket, i);
            REQUIRE(value[0] == i);
            REQUIRE(value[1] == i * 10);
            REQUIRE(value[2] == i * 100);
        }
    }

    SECTION("Can get and set values of two buffers")
    {
        int n = 4;
        DataBuffer d(n);

        SocketCollection<OutputSocket> sockets(nullptr);

        sockets.add(std::unique_ptr<OutputSocket>(new OutputSocket("position", ConnectionDataType::value<float, 3>(), SocketType::attribute)));

        sockets.add(std::unique_ptr<OutputSocket>(new OutputSocket("color", ConnectionDataType::value<unsigned char, 3>(), SocketType::attribute)));

        d.add(sockets);

        REQUIRE(d.num_attributes() == 2);
        REQUIRE(d.allocated_attribute_memory() == (sizeof(int) * 3 * 4) + (sizeof(unsigned char) * 3 * 4));

        for(int i = 0; i < n; i++)
        {
            float positions[] = { static_cast<float>(i), static_cast<float>(i * 10), static_cast<float>(i * 100) };
            unsigned char colors[] = { (unsigned char)i, (unsigned char)i, (unsigned char)i };

            d.set_attribute<float, 3>(sockets["position"], i, positions);
            d.set_attribute<unsigned char, 3>(sockets["color"], i, colors);
        }

        for(int i = 0; i < n; i++)
        {
            auto positions = d.get_attribute<float, 3>(sockets["position"], i);
            auto colors = d.get_attribute<unsigned char, 3>(sockets["color"], i);

            REQUIRE(positions[0] == i);
            REQUIRE(positions[1] == i * 10);
            REQUIRE(positions[2] == i * 100);

            REQUIRE(colors[0] == i);
            REQUIRE(colors[1] == i);
            REQUIRE(colors[2] == i);
        }
    }

    SECTION("Can get and set uniform")
    {
        DataBuffer d(1);
        OutputSocket socket("int", ConnectionDataType::value<int, 3>(), SocketType::uniform);

        d.add_uniform(ConnectionDataType::value<int, 3>());

        int values[] = { 1, 2, 3 };

        d.set_uniform<int, 3>(socket, values);

        auto values_returned = d.get_uniform<int, 3>(socket);

        REQUIRE(values_returned[0] == 1);
        REQUIRE(values_returned[1] == 2);
        REQUIRE(values_returned[2] == 3);

        REQUIRE(d.allocated_uniform_memory() == sizeof(int) * 3);
        REQUIRE(d.num_uniforms() == 1);
    }

    SECTION("Can get and set multiple uniforms")
    {
        DataBuffer d(1);
        SocketCollection<OutputSocket> sockets(nullptr);

        sockets.add(std::unique_ptr<OutputSocket>(
            new OutputSocket("a", ConnectionDataType::value<int, 2>(), SocketType::uniform)));
        sockets.add(std::unique_ptr<OutputSocket>(
            new OutputSocket("b", ConnectionDataType::value<float, 1>(), SocketType::uniform)));

        d.add(sockets);

        int a[] = { 1, 2 };
        d.set_uniform<int, 2>(sockets["a"], a);

        float b = 1.5;
        d.set_uniform<float, 1>(sockets["b"], &b);

        auto a_ret = d.get_uniform<int, 2>(sockets["a"]);
        auto b_ret = d.get_uniform<float, 1>(sockets["b"]);

        REQUIRE(a_ret[0] == 1);
        REQUIRE(a_ret[1] == 2);
        REQUIRE(b_ret[0] == 1.5);
    }
}
