#include "attribute_buffer.h"
#include "composite_data_buffer.h"

namespace noises {
namespace nodes {
    AttributeBuffer::AttributeBuffer() : input_(nullptr), output_(nullptr), has_manual_output_type_(false)
    {
        outputs().add("Output", ConnectionDataType::undefined(), SocketType::attribute);
        inputs().add("Input", SocketType::attribute);

        input_ = &inputs()["Input"];
        output_ = &outputs()["Output"];

        input_->accepts(ConnectionDataType::any());
    }

    void AttributeBuffer::recalculate_sockets()
    {
        auto connection = input_->connection();

        if(has_manual_output_type_)
            return;

        output_->set_data_type(connection ? connection->get().data_type() : ConnectionDataType::undefined());
    }

    /** Execute_uniforms is used instead of execute_attributes because execute_uniforms can move all the data in one call. **/
    void AttributeBuffer::execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const
    {
        auto input_data = input.get_attribute_all_raw(*input_, output_->data_type());
        output.set_attribute_all_raw(*output_, output_->data_type(), std::get<0>(input_data), std::get<1>(input_data));
    }

    std::string AttributeBuffer::node_name() const
    {
        return "Attribute Buffer";
    }

    InputSocket& AttributeBuffer::input()
    {
        return *input_;
    }

    const InputSocket& AttributeBuffer::input() const
    {
        return *input_;
    }

    OutputSocket& AttributeBuffer::output()
    {
        return *output_;
    }

    const OutputSocket& AttributeBuffer::output() const
    {
        return *output_;
    }

    void AttributeBuffer::set_output_type(const ConnectionDataType &data_type, AttributeInfo attribute_info)
    {
        output_->set_data_type(data_type);

        has_manual_output_type_ = true;
        override_attribute_info(attribute_info);
    }
} }
