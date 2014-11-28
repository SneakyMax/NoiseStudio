#include "constant_value.h"

namespace noises {
namespace nodes
{
    ConstantValue::ConstantValue() : value_socket_(nullptr)
    {
        value_socket_ = &outputs().add(socket_name, ConnectionDataType::undefined(), SocketType::uniform);
    }

    void ConstantValue::execute_uniforms(const CompositeDataBuffer&, DataBuffer &output) const
    {
        output.set_uniform_raw(*value_socket_, value_socket_->data_type(), &buffer_[0]);
    }

    const std::string ConstantValue::socket_name = "Value";
} }
