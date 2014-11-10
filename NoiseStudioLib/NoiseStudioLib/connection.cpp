#include <exception>
#include "connection.h"
#include "input_socket.h"
#include "output_socket.h"

namespace noises
{
    bool Connection::is_valid() const
    {
        return has_input_output() && input_output_type_compatible();
    }

    bool Connection::has_input_output() const
    {
        return input_.lock() && output_.lock();
    }

    bool Connection::input_output_type_compatible() const
    {
        std::shared_ptr<OutputSocket> output = output_.lock();
        std::shared_ptr<InputSocket> input = input_.lock();

        if(!input || !output)
            return false;

        return input->accepts(output->data_type());
    }

    void Connection::set_input(const std::shared_ptr<InputSocket>& socket)
    {
        input_ = socket;
    }

    void Connection::set_output(const std::shared_ptr<OutputSocket>& socket)
    {
        output_ = socket;
    }
}
