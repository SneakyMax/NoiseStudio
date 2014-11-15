#include "uniform_buffer.h"
#include "composite_data_buffer.h"

namespace noises {
namespace nodes
{
    UniformBuffer::UniformBuffer()
    {
        outputs().add("Output", ConnectionDataType::undefined(), SocketType::Uniform);
        inputs().add("Input", SocketType::Uniform);

        input_ = &inputs()["Input"];
        output_ = &outputs()["Output"];

        input_->set_accepts(ConnectionDataType::any());
    }

    void UniformBuffer::recalculate_sockets()
    {
        auto connection = input_->connection();

        output_->set_data_type(connection ? connection->get().data_type() : ConnectionDataType::undefined());
    }

    void UniformBuffer::execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const
    {
        output.set_uniform_raw(*output_, output_->data_type(), input.get_uniform_raw(*input_, output_->data_type()));
    }

    std::string UniformBuffer::node_name() const
    {
        return "Uniform Buffer";
    }

    InputSocket& UniformBuffer::input()
    {
        return *input_;
    }

    const InputSocket& UniformBuffer::input() const
    {
        return *input_;
    }

    OutputSocket& UniformBuffer::output()
    {
        return *output_;
    }

    const OutputSocket& UniformBuffer::output() const
    {
        return *output_;
    }
} }

