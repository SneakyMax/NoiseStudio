#include "uniform_buffer.h"
#include "composite_data_buffer.h"

namespace noises {
namespace nodes
{
    UniformBuffer::UniformBuffer() : input_(nullptr), output_(nullptr), has_manual_output_type_(false)
    {
        outputs().add("Output", ConnectionDataType::undefined(), SocketType::uniform);
        inputs().add("Input", SocketType::uniform);

        input_ = &inputs()["Input"];
        output_ = &outputs()["Output"];

        input_->set_accepts(ConnectionDataType::any());
    }

    void UniformBuffer::recalculate_sockets()
    {
        auto connection = input_->connection();

        if(has_manual_output_type_)
            return;

        output_->set_data_type(connection ? connection->get().data_type() : ConnectionDataType::undefined());
    }

    void UniformBuffer::execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const
    {
        output.set_uniform_raw(*output_, output_->data_type(), input.get_uniform_raw(*input_, input.get_uniform_type(0)));
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

    void UniformBuffer::set_output_type(const ConnectionDataType &data_type)
    {
        has_manual_output_type_ = true;
        output_->set_data_type(data_type);
    }
} }

