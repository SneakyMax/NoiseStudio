#include "math.h"

#include "composite_data_buffer.h"
#include "validation_results.h"
#include "output_socket.h"

namespace
{
    using namespace noises;
    using namespace nodes;

    template<typename T>
    T scalar_binary_math_operation(MathOperation operation, T a, T b)
    {
        switch(operation)
        {
            case MathOperation::add:
                return a + b;
            case MathOperation::subtract:
                return a - b;
            case MathOperation::multiply:
                return a * b;
            case MathOperation::divide:
                if(b == static_cast<T>(0))
                    return static_cast<T>(0);
                return b;
            case MathOperation::mod:
                return static_cast<int>(a) % static_cast<int>(b);
            case MathOperation::logorithm:
                return std::log(a) / std::log(b);

            default: return static_cast<T>(0);
        }
    }

    template<typename T>
    T scalar_unary_math_operation(MathOperation operation, T a)
    {
        switch(operation)
        {
            case MathOperation::negate:
                return -a;
            case MathOperation::square_root:
                return std::sqrt(a);
            case MathOperation::cube_root:
                return std::cbrt(a);
            case MathOperation::log_e:
                return std::log(a);
            case MathOperation::log_10:
                return std::log10(a);
            case MathOperation::log_2:
                return std::log2(a);
            case MathOperation::ceiling:
                return std::ceil(a);
            case MathOperation::floor:
                return std::floor(a);
            case MathOperation::round:
                return std::round(a);
            case MathOperation::sin:
                return std::sin(a);
            case MathOperation::cos:
                return std::cos(a);
            case MathOperation::tan:
                return std::tan(a);
            case MathOperation::asin:
                return std::asin(a);
            case MathOperation::acos:
                return std::acos(a);
            case MathOperation::atan:
                return std::atan(a);

            default: return static_cast<T>(0);
        }
    }

    template<typename T>
    std::vector<T> vector_binary_math_operation(MathOperation operation, const ConnectionDataType& data_type, const T* a, const T* b)
    {
        std::vector<T> out(data_type.dimensions());
        bool go_out = false;

        bool should_break = false;
        for(int i = 0; i < data_type.dimensions(); i++)
        {
            switch(operation)
            {
                case MathOperation::add:
                    out[i] = a[i] + b[i];
                    break;
                case MathOperation::subtract:
                    out[i] = a[i] - b[i];
                    break;
                case MathOperation::multiply:
                    //vectors can't be multiplied...but might as well allow the hammard product as well as other operations on individual components
                    out[i] = a[i] * b[i];
                    break;
                case MathOperation::divide:
                    if(b[i] == 0)
                        out[i] = 0;
                    else
                        out[i] = a[i] / b[i];
                    break;
                case MathOperation::exponent:
                    out[i] = std::pow(a[i], b[i]);
                    break;
                case MathOperation::logorithm:
                    out[i] = std::log(a[i]) / std::log(b[i]);
                    break;
                case MathOperation::mod:
                    out[i] = static_cast<int>(a[i]) % static_cast<int>(b[i]);
                    break;
                case MathOperation::atan2:
                    //how does this even make sense!?
                    out[i] = std::atan2(a[i], b[i]);
                    break;

                default: should_break = true; break;
            }

            if(should_break)
                break;

            go_out = true;
        }

        if(go_out)
            return out;

        if(operation == MathOperation::cross)
        {
            //TODO! only works for 3-dimensional vectors
            return out;
        }
        else if(operation == MathOperation::dot)
        {
            T sum = static_cast<T>(0);
            for(int i = 0; i < data_type.dimensions(); i++)
            {
                sum += a[i] * b[i];
            }
            out[0] = sum; //just put it in 0, the node will handle it
        }

        return out;
    }

    template<typename T>
    std::vector<T> vector_scalar_binary_math_operation(MathOperation operation, const ConnectionDataType& data_type, const T* a, const T b)
    {
        std::vector<T> out(data_type.dimensions());

        bool should_break = false;

        for(int i = 0; i < data_type.dimensions(); i++)
        {
            switch(operation)
            {
                case MathOperation::add:
                    out[i] = a[i] + b;
                    break;
                case MathOperation::subtract:
                    out[i] = a[i] - b;
                    break;
                case MathOperation::multiply:
                    out[i] = a[i] * b;
                    break;
                case MathOperation::divide:
                    if(b == 0)
                        out[i] = 0;
                    else
                        out[i] = a[i] / b;
                    break;
                case MathOperation::exponent:
                    out[i] = std::pow(a[i], b);
                    break;
                case MathOperation::logorithm:
                    out[i] = std::log(a[i]) / std::log(b);
                    break;
                case MathOperation::mod:
                    out[i] = static_cast<int>(a[i]) % static_cast<int>(b);
                    break;
                case MathOperation::atan2:
                    out[i] = std::atan2(a[i], b);
                    break;

                default: should_break = true; break;
            }

            if(should_break)
                break;
        }

        return out;
    }

    template<typename T>
    std::vector<T> vector_unary_math_operation(MathOperation operation, const ConnectionDataType& data_type, const T* a)
    {
        std::vector<T> out(data_type.dimensions());

        bool should_break = false;

        for(int i = 0; i < data_type.dimensions(); i++)
        {
            switch(operation)
            {
                case MathOperation::negate:
                    out[i] = -a[i];
                    break;
                case MathOperation::square_root:
                    out[i] = std::sqrt(a[i]);
                    break;
                case MathOperation::cube_root:
                    out[i] = std::cbrt(a[i]);
                    break;
                case MathOperation::log_e:
                    out[i] = std::log(a[i]);
                    break;
                case MathOperation::log_10:
                    out[i] = std::log10(a[i]);
                    break;
                case MathOperation::log_2:
                    out[i] = std::log2(a[i]);
                    break;
                case MathOperation::ceiling:
                    out[i] = std::ceil(a[i]);
                    break;
                case MathOperation::floor:
                    out[i] = std::floor(a[i]);
                    break;
                case MathOperation::round:
                    out[i] = std::round(a[i]);

                default: should_break = true; break;
            }

            if(should_break)
                break;
        }

        return out;
    }

    template<typename T>
    void execute_math_operation_binary(MathOperation operation,
                                const InputSocket& a, const InputSocket& b,
                                const OutputSocket& output_socket,
                                const CompositeDataBuffer& input, DataBuffer& output,
                                DataBuffer::size_type index = 0)
    {
        const ConnectionDataType& a_type = a.connection()->get().data_type();
        const ConnectionDataType& b_type = b.connection()->get().data_type();

        SocketType a_socket_type = a.connection()->get().output().type();
        SocketType b_socket_type = b.connection()->get().output().type();

        // two scalars
        if(a_type.dimensions() == 1 && b_type.dimensions() == 1)
        {

            ptr_array<T, 1> a_value;
            ptr_array<T, 1> b_value;

            if(a_socket_type == SocketType::attribute)
            {
                a_value = input.get_attribute<T, 1>(a, index);
            }
            else
            {
                a_value = input.get_uniform<T, 1>(a);
            }

            if(b_socket_type == SocketType::attribute)
            {
                b_value = input.get_attribute<T, 1>(b, index);
            }
            else
            {
                b_value = input.get_uniform<T, 1>(b);
            }

            T result = scalar_binary_math_operation(operation, a_value.value(), b_value.value());

            if(output_socket.type() == SocketType::attribute)
            {
                output.set_attribute<T, 1>(output_socket, index, &result);
            }
            else
            {
                output.set_uniform<T, 1>(output_socket, &result);
            }
        }
        // scalar and a vector
        else if((a_type.dimensions() > 1 && b_type.dimensions() == 1) || (a_type.dimensions() == 1 && b_type.dimensions() > 1))
        {
            // a and b are arbitrary here
            const T* a_value;
            ptr_array<T, 1> b_value;

            // This large if mess is just getting data into a_value and b_value
            if(a_type.dimensions() == 1)
            {
                if(a_socket_type == SocketType::attribute)
                {
                    b_value = input.get_attribute<T, 1>(a, index);
                }
                else
                {
                    b_value = input.get_uniform<T, 1>(a);
                }

                if(b_socket_type == SocketType::attribute)
                {
                    a_value = reinterpret_cast<const T*>(input.get_attribute_raw(b, b_type, index));
                }
                else
                {
                    a_value = reinterpret_cast<const T*>(input.get_uniform_raw(b, b_type));
                }

                std::vector<T> result = vector_scalar_binary_math_operation(operation, b_type, a_value, b_value.value());

                if(output_socket.type() == SocketType::attribute)
                {
                    output.set_attribute_raw(output_socket, b_type, index, reinterpret_cast<unsigned char*>(&result[0]));
                }
                else
                {
                    output.set_uniform_raw(output_socket, b_type, reinterpret_cast<unsigned char*>(&result[0]));
                }
            }
            else
            {
                if(b_socket_type == SocketType::attribute)
                {
                    b_value = input.get_attribute<T, 1>(b, index);
                }
                else
                {
                    b_value = input.get_uniform<T, 1>(b);
                }

                if(a_socket_type == SocketType::attribute)
                {
                    a_value = reinterpret_cast<const T*>(input.get_attribute_raw(a, a_type, index));
                }
                else
                {
                    a_value = reinterpret_cast<const T*>(input.get_uniform_raw(a, a_type));
                }

                std::vector<T> result = vector_scalar_binary_math_operation(operation, a_type, a_value, b_value.value());

                if(output_socket.type() == SocketType::attribute)
                {
                    output.set_attribute_raw(output_socket, a_type, index, reinterpret_cast<unsigned char*>(&result[0]));
                }
                else
                {
                    output.set_uniform_raw(output_socket, a_type, reinterpret_cast<unsigned char*>(&result[0]));
                }
            }
        }
        // two vectors
        else if(a_type.dimensions() > 1 && b_type.dimensions() > 1)
        {
            const T* a_value;
            const T* b_value;

            if(a_socket_type == SocketType::attribute)
            {
                a_value = reinterpret_cast<const T*>(input.get_attribute_raw(a, a_type, index));
            }
            else
            {
                a_value = reinterpret_cast<const T*>(input.get_uniform_raw(a, a_type));
            }

            if(b_socket_type == SocketType::attribute)
            {
                b_value = reinterpret_cast<const T*>(input.get_attribute_raw(b, b_type, index));
            }
            else
            {
                b_value = reinterpret_cast<const T*>(input.get_uniform_raw(b, b_type));
            }

            std::vector<T> result = vector_binary_math_operation(operation, a_type, a_value, b_value);

            if(output_socket.type() == SocketType::attribute)
            {
                output.set_attribute_raw(output_socket, a_type, index, reinterpret_cast<unsigned char*>(&result[0]));
            }
            else
            {
                output.set_uniform_raw(output_socket, a_type, reinterpret_cast<unsigned char*>(&result[0]));
            }
        }
        else throw std::logic_error("What happened?");
    }

    template<typename T>
    void execute_math_operation_unary(MathOperation operation,
                                      const InputSocket& a,
                                      const OutputSocket& output_socket,
                                      const CompositeDataBuffer& input, DataBuffer& output,
                                      DataBuffer::size_type index = 0)
    {
        const ConnectionDataType& a_type = a.connection()->get().data_type();
        SocketType a_socket_type = a.connection()->get().output().type();
        bool is_attribute = a_socket_type == SocketType::attribute;

        if(a_type.dimensions() == 1)
        {
            ptr_array<T, 1> a_value;

            if(is_attribute)
                a_value = input.get_attribute<T, 1>(a, index);
            else
                a_value = input.get_uniform<T, 1>(a);

            T result = scalar_unary_math_operation(operation, a_value.value());

            if(is_attribute)
                output.set_attribute<T, 1>(output_socket, index, &result);
            else
                output.set_uniform<T, 1>(output_socket, &result);
        }
        else
        {
            const T* a_value;

            if(is_attribute)
                a_value = reinterpret_cast<const T*>(input.get_attribute_raw(a, a_type, index));
            else
                a_value = reinterpret_cast<const T*>(input.get_uniform_raw(a, a_type));

            std::vector<T> result = vector_unary_math_operation(operation, a_type, a_value);

            if(is_attribute)
                output.set_attribute_raw(output_socket, a_type, index, reinterpret_cast<const unsigned char*>(&result[0]));
            else
                output.set_uniform_raw(output_socket, a_type, reinterpret_cast<const unsigned char*>(&result[0]));
        }
    }
}

namespace noises {
namespace nodes {
    Math::Math()
    {
        Property& operation = add_property<int, 1>("Operation");
        int default_operation = (int)MathOperation::add;
        operation.set_default_value<int, 1>(&default_operation);

        inputs().add("A", SocketType::either);
        inputs().add("B", SocketType::either);

        outputs().add("Output", ConnectionDataType::undefined(), SocketType::uniform);

        input("A").set_accepts(ConnectionDataType::any()); // we do better validation in validate
        input("B").set_accepts(ConnectionDataType::any());
    }

    void Math::recalculate_sockets()
    {
        if(!inputs().get_by_name("A"))
            return; //Still initializing

        recalculate_num_sockets();

        if(inputs().get_by_name("B"))
        {
            recalculate_sockets_binary();
        }
        else
        {
            recalculate_sockets_unary();
        }
    }

    void Math::recalculate_num_sockets()
    {
        Property& operation_prop = property("Operation");
        MathOperation operation = static_cast<MathOperation>(operation_prop.value_or_default<int, 1>().value());

        // Everything before negate is binary, everything after is unary
        if(static_cast<int>(operation) >= static_cast<int>(MathOperation::negate))
        {
            should_not_have_b();
        }
        else
        {
            should_have_b();
        }
    }

    void Math::should_have_b()
    {
        if(inputs().get_by_name("B"))
            return;

        inputs().add("B", SocketType::either);
        input("B").set_accepts(ConnectionDataType::any());
    }

    void Math::should_not_have_b()
    {
        if(!inputs().get_by_name("B"))
            return;

        inputs().remove(input("B"));
    }

    void Math::recalculate_sockets_binary()
    {
        // Match the output type to the input type
        auto a_connection = input("A").connection();
        auto b_connection = input("B").connection();

        if(a_connection)
        {
            const Connection& connection = *a_connection;
            output("Output").set_data_type(connection.data_type());
            output("Output").set_type(connection.output().type());
        }
        else if(b_connection)
        {
            const Connection& connection = *b_connection;
            output("Output").set_data_type(connection.data_type());
            output("Output").set_type(connection.output().type());
        }
        else
        {
            output("Output").set_data_type(ConnectionDataType::undefined());
        }

        if(a_connection && b_connection)
        {
            const Connection& a_connection_val = *a_connection;
            const Connection& b_connection_val = *b_connection;

            // if *either* connection is an attribute, the result is an attribute
            if(a_connection_val.output().type() == SocketType::uniform ||
               b_connection_val.output().type() == SocketType::uniform)
            {
                output("Output").set_type(SocketType::uniform);
            }

            // set the output vector size to the larger of the vector sizes. If they're both >1,
            // then validate with reject it. If they're the same size it won't matter, and if it's a scalar
            // and a vector, the vector will take presidence
            if(a_connection_val.data_type().dimensions() >= b_connection_val.data_type().dimensions())
            {
                output("Output").set_data_type(a_connection_val.data_type());
            }
            else
            {
                output("Output").set_data_type(b_connection_val.data_type());
            }
        }
    }

    void Math::recalculate_sockets_unary()
    {
        auto a_connection = input("A").connection();

        if(a_connection)
        {
            const Connection& connection = *a_connection;
            output("Output").set_data_type(connection.data_type());
            output("Output").set_type(connection.output().type());
        }
        else
        {
            output("Output").set_data_type(ConnectionDataType::undefined());
        }
    }

    void Math::execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const
    {
        if(this->output("Output").type() == SocketType::attribute)
            return; // Handled in execute_attributes

        const Property& operation_prop = property("Operation");

        MathOperation operation = static_cast<MathOperation>(operation_prop.value_or_default<int, 1>().value());

        const OutputSocket& output_socket = this->output("Output");

        const InputSocket& a_socket = this->input("A");
        const ConnectionDataType& a_type = a_socket.connection()->get().data_type();

        if(inputs().get_by_name("B"))
        {
            const InputSocket& b_socket = this->input("B");

            if(a_type.is<int>())
            {
                execute_math_operation_binary<int>(operation, a_socket, b_socket, output_socket, input, output);
            }
            else if(a_type.is<float>())
            {
                execute_math_operation_binary<float>(operation, a_socket, b_socket, output_socket, input, output);
            }
            else if(a_type.is<double>())
            {
                execute_math_operation_binary<double>(operation, a_socket, b_socket, output_socket, input, output);
            }
        }
        else
        {
            if(a_type.is<int>())
            {
                execute_math_operation_unary<int>(operation, a_socket, output_socket, input, output);
            }
            else if(a_type.is<float>())
            {
                execute_math_operation_unary<float>(operation, a_socket, output_socket, input, output);
            }
            else if(a_type.is<double>())
            {
                execute_math_operation_unary<double>(operation, a_socket, output_socket, input, output);
            }
        }
    }

    void Math::execute_attributes(const CompositeDataBuffer &input, DataBuffer &output, DataBuffer::size_type index) const
    {
        if(this->output("Output").type() == SocketType::uniform)
            return; //Handled in execute_uniforms

        const Property& operation_prop = property("Operation");

        MathOperation operation = static_cast<MathOperation>(operation_prop.value_or_default<int, 1>().value());

        const OutputSocket& output_socket = this->output("Output");

        const InputSocket& a_socket = this->input("A");
        const InputSocket& b_socket = this->input("B");

        const ConnectionDataType& a_type = a_socket.connection()->get().data_type();

        if(inputs().get_by_name("B"))
        {
            if(a_type.is<int>())
            {
                execute_math_operation_binary<int>(operation, a_socket, b_socket, output_socket, input, output, index);
            }
            else if(a_type.is<float>())
            {
                execute_math_operation_binary<float>(operation, a_socket, b_socket, output_socket, input, output, index);
            }
            else if(a_type.is<double>())
            {
                execute_math_operation_binary<double>(operation, a_socket, b_socket, output_socket, input, output, index);
            }
        }
        else
        {
            if(a_type.is<int>())
            {
                execute_math_operation_unary<int>(operation, a_socket, output_socket, input, output, index);
            }
            else if(a_type.is<float>())
            {
                execute_math_operation_unary<float>(operation, a_socket, output_socket, input, output, index);
            }
            else if(a_type.is<double>())
            {
                execute_math_operation_unary<double>(operation, a_socket, output_socket, input, output, index);
            }
        }


    }

    std::string Math::node_name() const
    {
        return "Math";
    }

    void Math::validate(ValidationResults &results) const
    {
        if(inputs().get_by_name("B"))
        {
            const InputSocket& a_socket = input("A");
            const InputSocket& b_socket = input("B");

            auto a_connection = a_socket.connection();
            auto b_connection = b_socket.connection();

            if(!a_connection || !b_connection)
                return; // Validation will be handled by the global validator

            const ConnectionDataType& a_type = a_connection->get().data_type();
            const ConnectionDataType& b_type = b_connection->get().data_type();

            if((a_type.is<float>() || a_type.is<int>() || a_type.is<double>()) == false ||
               (b_type.is<float>() || b_type.is<int>() || b_type.is<double>()) == false)
            {
                results.add("Math node only supports float, int, or double data types (uniform or attribute)");
            }

            if(a_type.is_same_type(b_type) == false)
            {
                results.add("Math node A and B must be the same data type (not neccesarily dimension size too). Try using converter nodes.");
            }

            if(a_type.dimensions() == 0 || b_type.dimensions() == 0)
            {
                results.add("An input is dimensionless.");
            }

            if((a_type.dimensions() == 1 || b_type.dimensions() == 1) == false && a_type.dimensions() != b_type.dimensions())
            {
                results.add("Math node supports math operations on two scalars, two same-sized vectors, or one vector and one scalar.");
            }
        }
        else
        {
            const InputSocket& a_socket = input("A");
            auto a_connection = a_socket.connection();
            const ConnectionDataType& a_type = a_connection->get().data_type();

            if((a_type.is<float>() || a_type.is<int>() || a_type.is<double>()) == false)
            {
                results.add("Math node only supports float, int, or double data types (uniform or attribute)");
            }

            if(a_type.dimensions() == 0)
            {
                results.add("An input is dimensionless.");
            }
        }

    }
} }


