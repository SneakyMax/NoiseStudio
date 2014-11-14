#ifndef CONSTANT_VALUE_H
#define CONSTANT_VALUE_H

#include <vector>

#include "graph_node.h"

#include "connection_data_type.h"

namespace noises {
namespace nodes
{
    class ConstantValue : public GraphNode
    {
    public:
        ConstantValue();

        std::string node_name() const { return "Constant Value"; }

        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> value() const
        {
            return buffer_;
        }

        template<typename T, unsigned int Dimensions>
        void set_value(const ptr_array<T, Dimensions> value)
        {
            value.copy_to(buffer_);

            create_value_if_not_exist<T, Dimensions>();
            outputs().get_by_name("value").get().get().set_data_type(ConnectionDataType::value<T, Dimensions>());
        }

        void execute_uniforms(const CompositeDataBuffer& input, DataBuffer& output);

    private:
        template<typename T, unsigned int Dimensions>
        void create_value_if_not_exist()
        {
            if(outputs().get_by_name("value"))
                return;

            value_socket_ = &outputs().add("value", ConnectionDataType::value<T, Dimensions>(), SocketType::Uniform);
        }

        std::vector<unsigned char> buffer_;

        OutputSocket* value_socket_;
    };
} }



#endif // CONSTANT_VALUE_H
