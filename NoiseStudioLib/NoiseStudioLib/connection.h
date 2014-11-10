#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>

namespace noises
{
    class InputSocket;
    class OutputSocket;

    class Connection
    {
    public:
        // Gets whether the connection is valid (has an input and output, and the input accepts the value type of the output)
        // the connection should be deleted
        bool is_valid() const;

        bool input_output_type_compatible() const;
        bool has_input_output() const;

        void set_input(const std::shared_ptr<InputSocket>& socket);
        void set_output(const std::shared_ptr<OutputSocket>& socket);
    private:

        // The input is generally the *right* side of a connection
        std::weak_ptr<InputSocket> input_;

        // The output is generally the *left* side of a connection
        std::weak_ptr<OutputSocket> output_;
    };
}

#endif // CONNECTION_H
