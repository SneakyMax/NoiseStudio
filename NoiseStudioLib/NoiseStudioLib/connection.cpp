#include <exception>
#include "connection.h"
#include "input_socket.h"
#include "output_socket.h"

namespace noises
{
    bool Connection::input_output_type_compatible() const
    {
        return input_.accepts(output_.data_type());
    }

    const InputSocket& Connection::input() const
    {
        return input_;
    }

    const OutputSocket& Connection::output() const
    {
        return output_;
    }

    void Connection::disconnect()
    {
        input_.set_connection(nullptr);
        output_.remove_connection(*this);
    }
}
