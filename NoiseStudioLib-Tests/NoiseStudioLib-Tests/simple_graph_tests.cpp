#include "catch.h"

#include <graph.h>
#include <nodes/constant_value.h>
#include <graph_executor.h>

using namespace noises;

TEST_CASE("Simple graph, can get output", "")
{
    Graph graph;

    std::unique_ptr<nodes::ConstantValue> cv(new nodes::ConstantValue);
    int value = 7;
    cv->set_value<int, 1>(&value);

    int cv_id = graph.add_node(std::move(cv));
    graph.add_uniform_output("int");

    graph.connect(graph.get_node_by_id(cv_id)->get().outputs()[nodes::ConstantValue::socket_name],
                  graph.get_uniform_output("int")->get().input());

    GraphExecutor e(graph);

    GraphOutputs out(e.execute());
}
