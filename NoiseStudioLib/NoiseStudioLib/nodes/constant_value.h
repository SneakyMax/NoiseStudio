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
        static const std::string socket_name;

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
            outputs()[socket_name].set_data_type(ConnectionDataType::value<T, Dimensions>());
        }

        /** Convenience method for setting a <T, 1> value **/
        template<typename T>
        void set_value_single(T value)
        {
            set_value<T, 1>(&value);
        }

        void execute_uniforms(const CompositeDataBuffer& input, DataBuffer& output) const;

    private:
        template<typename T, unsigned int Dimensions>
        void create_value_if_not_exist()
        {
            if(outputs().get_by_name(socket_name))
                return;

            value_socket_ = &outputs().add(socket_name, ConnectionDataType::value<T, Dimensions>(), SocketType::Uniform);
        }

        std::vector<unsigned char> buffer_;

        OutputSocket* value_socket_;
    };
} }



#endif // CONSTANT_VALUE_H
