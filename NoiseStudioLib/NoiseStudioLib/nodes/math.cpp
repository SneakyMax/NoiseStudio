#include "math.h"

#include "composite_data_buffer.h"
#include "validation_results.h"

namespace
{
    template<typename T>
    T math_operation(noises::nodes::MathOperation operation, T a, T b)
    {
        if(operation == noises::nodes::MathOperation::add)
        {
            return a + b;
        }
        else if(operation == noises::nodes::MathOperation::subtract)
        {
            return a - b;
        }
        else if(operation == noises::nodes::MathOperation::multiply)
        {
            return a * b;
        }
        else if(operation == noises::nodes::MathOperation::divide)
        {
            if(b == 0)
                return 0;
            return a / b;
        }
        else if(operation == noises::nodes::MathOperation::exponent)
        {
            return std::pow(a, b);
        }

        else return 0;
    }
}

namespace noises {
namespace nodes {
    Math::Math()
    {
        inputs().add("A", SocketType::Uniform);
        inputs().add("B", SocketType::Uniform);

        outputs().add("Output", ConnectionDataType::undefined(), SocketType::Uniform);

        // For vector math, use vector math node...
        inputs()["A"].set_accepts(ConnectionDataType::value<int, 1>());
        inputs()["A"].set_accepts(ConnectionDataType::value<float, 1>());
        inputs()["A"].set_accepts(ConnectionDataType::value<double, 1>());

        inputs()["B"].set_accepts(ConnectionDataType::value<int, 1>());
        inputs()["B"].set_accepts(ConnectionDataType::value<float, 1>());
        inputs()["B"].set_accepts(ConnectionDataType::value<double, 1>());

        Property& operation = add_property<int, 1>("Operation");
        int default_operation = (int)MathOperation::add;
        operation.set_default_value<int, 1>(&default_operation);
    }

    void Math::recalculate_sockets()
    {
        // Match the output type to the input type
        if(inputs()["A"].connection())
        {
            outputs()["Output"].set_data_type(inputs()["A"].connection()->get().data_type());
        }
        else if(inputs()["B"].connection())
        {
            outputs()["Output"].set_data_type(inputs()["B"].connection()->get().data_type());
        }
    }

    void Math::execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const
    {
        const Property& operation_prop = *get_property_by_name("Operation");

        MathOperation operation = static_cast<MathOperation>(operation_prop.value_or_default<int, 1>().value());

        const OutputSocket& output_socket = outputs()["Output"];

        const InputSocket& a_socket = inputs()["A"];
        const InputSocket& b_socket = inputs()["B"];

        const ConnectionDataType& a_type = a_socket.connection()->get().data_type();

        if(a_type != b_socket.connection()->get().data_type())
            return; //TODO nodes should have a validate() function

        if(a_type.is<int>())
        {
            int result = math_operation<int>(operation, input.get_uniform<int, 1>(a_socket), input.get_uniform<int, 1>(b_socket));
            output.set_uniform<int, 1>(output_socket, &result);
        }
        else if(a_type.is<float>())
        {
            float result = math_operation<float>(operation, input.get_uniform<float, 1>(a_socket), input.get_uniform<int, 1>(b_socket));
            output.set_uniform<float, 1>(output_socket, &result);
        }
        else if(a_type.is<double>())
        {
            double result = math_operation<double>(operation, input.get_uniform<double, 1>(a_socket), input.get_uniform<double, 1>(b_socket));
            output.set_uniform<double, 1>(output_socket, &result);
        }
    }

    std::string Math::node_name() const
    {
        return "Math";
    }

    void Math::validate(ValidationResults &results) const
    {
        const InputSocket& a_socket = input("A");
        const InputSocket& b_socket = input("B");

        auto a_connection = a_socket.connection();
        auto b_connection = b_socket.connection();

        if(!a_connection || !b_connection)
            return; // Validation will be handled by the global validator

        const ConnectionDataType& a_type = a_connection->get().data_type();
        const ConnectionDataType& b_type = b_connection->get().data_type();

        if(a_type != b_type)
        {
            results.add("Math node requires A and B to be the same data type. Try using converter nodes.");
        }
    }
} }


