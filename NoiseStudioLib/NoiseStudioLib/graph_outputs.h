#ifndef EXECUTED_GRAPH_H
#define EXECUTED_GRAPH_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "data_buffer.h"

namespace noises
{
    class GraphOutputs
    {
    public:
        GraphOutputs();
        GraphOutputs(const GraphOutputs&) = delete;
        GraphOutputs& operator=(const GraphOutputs&) = delete;
        GraphOutputs(GraphOutputs&&) = default;
        GraphOutputs& operator=(GraphOutputs&&) = default;

        DataBuffer& get(const std::string& name);

        void add(const std::string& name, std::unique_ptr<DataBuffer> buffer);

        std::vector<std::pair<std::string, std::reference_wrapper<DataBuffer>>> buffers();

    private:

        std::unordered_map<std::string, std::reference_wrapper<DataBuffer>> buffer_map_;
        std::vector<std::unique_ptr<DataBuffer>> buffers_;
    };
}



#endif // EXECUTED_GRAPH_H
