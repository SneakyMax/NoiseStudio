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
        Connection(InputSocket& input, OutputSocket& output)
            : input_(input), output_(output) { }

        bool input_output_type_compatible() const;

        const InputSocket& input() const;
        const OutputSocket& output() const;

        void disconnect();
    private:

        // The input is generally the *right* side of a connection
        InputSocket& input_;

        // The output is generally the *left* side of a connection
        OutputSocket& output_;
    };
}

#endif // CONNECTION_H
