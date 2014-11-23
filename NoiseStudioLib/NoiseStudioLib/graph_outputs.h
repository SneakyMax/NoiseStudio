#ifndef EXECUTED_GRAPH_H
#define EXECUTED_GRAPH_H

#include <vector>
#include <memory>
#include <unordered_map>

#include "data_buffer.h"
#include "attribute_info.h"

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

        template<typename ValueType, unsigned int Dimensions>
        const ptr_array<ValueType, Dimensions> get_uniform(const std::string& name) const
        {
            DataBuffer& buffer = get_raw_buffer(name);
            auto& type = ConnectionDataType::value<ValueType, Dimensions>();
            assert(buffer.get_uniform_type(0) == type);
            return buffer.get_uniform_raw(0);
        }

        template<typename ValueType, unsigned int Dimensions>
        const ptr_array<ValueType, Dimensions> get_attribute(const std::string& name, DataBuffer::size_type index) const
        {
            DataBuffer& buffer = get_raw_buffer(name);
            auto& type = ConnectionDataType::value<ValueType, Dimensions>();
            assert(buffer.get_attribute_type(0) == type);
            return buffer.get_attribute_raw(0, index);
        }

        /** Gets the entire contents of an attribute block. Returns a pointer to the first member, which has (attribute_length()) length. **/
        template<typename ValueType>
        const ValueType* get_attribute_all(const std::string& name) const
        {
            DataBuffer& buffer = get_raw_buffer(name);
            assert(buffer.get_attribute_type(0).is<ValueType>());
            return reinterpret_cast<ValueType*>(&buffer.get_memory_block(0)[0]);
        }

        AttributeInfo attribute_info(const std::string& name) const;

        DataBuffer& get_raw_buffer(const std::string& name) const;

        void add(const std::string& name, std::unique_ptr<DataBuffer> buffer);

        std::vector<std::pair<std::string, std::reference_wrapper<DataBuffer>>> buffers();

    private:

        std::unordered_map<std::string, std::reference_wrapper<DataBuffer>> buffer_map_;
        std::vector<std::unique_ptr<DataBuffer>> buffers_;
    };
}



#endif // EXECUTED_GRAPH_H
