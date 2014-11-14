#include "catch.h"

#include <graph.h>
#include <constant_value.h>

using namespace noises;

TEST_CASE("Simple graph", "")
{
    Graph graph;

    std::unique_ptr<nodes::ConstantValue> cv(new nodes::ConstantValue);
    int value = 7;
    cv->set_value<int, 1>(&value);

    graph.add(std::move(cv));

    auto& output = graph.add_output("int", ConnectionDataType::value<int, 1>(), SocketType::Uniform);
}
