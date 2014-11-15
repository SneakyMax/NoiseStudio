#include "catch.h"

#include <nodes/math.h>
#include <nodes/constant_value.h>
#include <graph.h>

using namespace noises;

TEST_CASE("Math node addition", "")
{
    Graph graph;

    std::unique_ptr<nodes::ConstantValue> value_1_node(new nodes::ConstantValue);
    std::unique_ptr<nodes::ConstantValue> value_2_node(new nodes::ConstantValue);

    std::unique_ptr<nodes::Math> math_node(new nodes::Math);
}
