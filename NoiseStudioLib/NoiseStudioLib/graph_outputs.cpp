#include "graph_outputs.h"

namespace noises
{
    GraphOutputs::GraphOutputs() { }

    DataBuffer& GraphOutputs::get(const std::string &name)
    {
        auto it = buffer_map_.find(name);
        if(it == buffer_map_.end())
            throw std::invalid_argument("Could not find an output named " + name);
        return std::get<1>(*it).get();
    }

    void GraphOutputs::add(const std::string& name, std::unique_ptr<DataBuffer> buffer)
    {
        auto ref = std::ref(*buffer);
        buffers_.push_back(std::move(buffer));
        buffer_map_.emplace(std::make_pair(name, ref));
    }

    std::vector<std::pair<std::string, std::reference_wrapper<DataBuffer>>> GraphOutputs::buffers()
    {
        std::vector<std::pair<std::string, std::reference_wrapper<DataBuffer>>> out;
        auto it = buffer_map_.begin();
        for(; it != buffer_map_.end(); ++it)
        {
            out.push_back(*it);
        }
        return out;
    }
}
