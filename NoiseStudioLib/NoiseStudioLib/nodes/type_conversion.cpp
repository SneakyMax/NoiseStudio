#include "type_conversion.h"

#include "composite_data_buffer.h"

namespace noises {
namespace nodes {

    template<typename TSource, typename TTarget>
    void change_type(const CompositeDataBuffer& input, DataBuffer& output, SocketType type, InputSocket& input_socket, OutputSocket& output_socket)
    {
        const Connection& connection = *input_socket.connection();
        const ConnectionDataType& input_type = connection.data_type();

        assert(input_type.is<TSource>());
        assert(output_socket.data_type().is<TTarget>());
        assert(input_type.dimensions() == output_socket.data_type().dimensions());

        if(type == SocketType::attribute)
        {
            std::tuple<const unsigned char*, std::size_t> input_data = input.get_attribute_all_raw(input_socket, input_type);
            const TSource* source = reinterpret_cast<const TSource*>(std::get<0>(input_data));
            std::size_t source_length = std::get<1>(input_data) / sizeof(TSource);

            std::vector<TTarget> out;
            out.reserve(sizeof(TTarget) *  source_length);
            for(std::size_t i = 0; i < source_length; i++)
            {
                out.push_back(static_cast<TTarget>(source[i]));
            }

            output.set_attribute_all_raw(output_socket, output_socket.data_type(), reinterpret_cast<const unsigned char*>(out.data()), out.size() * sizeof(TTarget));
        }
        else //if(type == SocketType::uniform)
        {
            const unsigned char* input_data = input.get_uniform_raw(input_socket, input_socket.connection()->get().data_type());
            std::size_t source_length = input_type.dimensions();
            const TSource* source = reinterpret_cast<const TSource*>(input_data);

            std::vector<TTarget> out;
            out.reserve(sizeof(TTarget) * input_type.dimensions());

            for(std::size_t i = 0; i < source_length; i++)
            {
                out.push_back(static_cast<TTarget>(source[i]));
            }

            output.set_uniform_raw(output_socket, output_socket.data_type(), reinterpret_cast<const unsigned char*>(out.data()));
        }
    }

    template<typename TSource>
    void change_type(const CompositeDataBuffer &input, DataBuffer &output, SocketType socket_type, InputSocket &input_socket, OutputSocket &output_socket)
    {
        const ConnectionDataType& output_type = output_socket.data_type();

        if(output_type.is<int>())
        {
            change_type<TSource, int>(input, output, socket_type, input_socket, output_socket);
        }
        else if(output_type.is<long>())
        {
            change_type<TSource, long>(input, output, socket_type, input_socket, output_socket);
        }
        else if(output_type.is<float>())
        {
            change_type<TSource, float>(input, output, socket_type, input_socket, output_socket);
        }
        else if(output_type.is<double>())
        {
            change_type<TSource, double>(input, output, socket_type, input_socket, output_socket);
        }
        else if(output_type.is<unsigned char>())
        {
            change_type<TSource, unsigned char>(input, output, socket_type, input_socket, output_socket);
        }
        else if(output_type.is<unsigned int>())
        {
            change_type<TSource, unsigned int>(input, output, socket_type, input_socket, output_socket);
        }
    }

    TypeConversion::TypeConversion() :
        input_socket_(nullptr),
        output_socket_(nullptr),
        type_property_(nullptr)
    {
        input_socket_ = &inputs().add("Input", SocketType::either);
        output_socket_ = &outputs().add("Output", ConnectionDataType::undefined(), SocketType::attribute);

        input_socket_->set_accepts(ConnectionDataType::any());

        type_property_ = &add_property<int, 1>("Target Type");

        int default_type = static_cast<int>(TypeConversionTargetType::int_t);
        type_property_->set_default_value<int, 1>(&default_type);
    }

    std::string TypeConversion::node_name() const
    {
        return "Type Conversion";
    }

    void TypeConversion::set_type(TypeConversionTargetType type)
    {
        int value = static_cast<int>(type);
        type_property_->set_value<int, 1>(&value);
    }

    void TypeConversion::execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const
    {
        if(!input_socket_->connection())
            throw std::invalid_argument("Missing input connection");

        const Connection& input_connection = *input_socket_->connection();
        const ConnectionDataType& input_type = input_connection.data_type();
        SocketType socket_type = output_socket_->type();

        if(input_type.is<int>())
        {
            change_type<int>(input, output, socket_type, *input_socket_, *output_socket_);
        }
        else if(input_type.is<long>())
        {
            change_type<long>(input, output, socket_type, *input_socket_, *output_socket_);
        }
        else if(input_type.is<float>())
        {
            change_type<float>(input, output, socket_type, *input_socket_, *output_socket_);
        }
        else if(input_type.is<double>())
        {
            change_type<double>(input, output, socket_type, *input_socket_, *output_socket_);
        }
        else if(input_type.is<unsigned char>())
        {
            change_type<unsigned char>(input, output, socket_type, *input_socket_, *output_socket_);
        }
        else if(input_type.is<unsigned int>())
        {
            change_type<unsigned int>(input, output, socket_type, *input_socket_, *output_socket_);
        }
    }

    void TypeConversion::recalculate_sockets()
    {
        if(input_socket_ == nullptr || output_socket_ == nullptr || type_property_ == nullptr)
            return; //Not initialized yet

        TypeConversionTargetType target_type = static_cast<TypeConversionTargetType>(type_property_->value_or_default<int, 1>().value());

        if(!input_socket_->connection())
        {
            output_socket_->set_data_type(get_type(target_type, 0));
            return;
        }

        const Connection& connection = *input_socket_->connection();
        const ConnectionDataType& output_type = get_type(target_type, connection.data_type().dimensions());

        SocketType socket_type = connection.output().type();
        output_socket_->set_type(socket_type);
        output_socket_->set_data_type(output_type);
    }

    const ConnectionDataType& TypeConversion::get_type(TypeConversionTargetType target, int dimensions)
    {
        switch(target)
        {
            case TypeConversionTargetType::int_t:
                return ConnectionDataType::dynamic<int>(dimensions);
            case TypeConversionTargetType::long_t:
                return ConnectionDataType::dynamic<long>(dimensions);
            case TypeConversionTargetType::float_t:
                return ConnectionDataType::dynamic<float>(dimensions);
            case TypeConversionTargetType::double_t:
                return ConnectionDataType::dynamic<double>(dimensions);
            case TypeConversionTargetType::uchar_t:
                return ConnectionDataType::dynamic<unsigned char>(dimensions);
            case TypeConversionTargetType::uint_t:
                return ConnectionDataType::dynamic<unsigned int>(dimensions);
        }

        throw std::invalid_argument("Unsupported type " + boost::lexical_cast<std::string>(static_cast<int>(target)));
    }

}}


