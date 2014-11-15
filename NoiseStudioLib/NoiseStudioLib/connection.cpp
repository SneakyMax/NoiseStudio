#include <exception>
#include "connection.h"
#include "input_socket.h"
#include "output_socket.h"

namespace noises
{
    Connection::Connection(InputSocket &input, OutputSocket &output, Graph &parent_graph) :
        input_(input), output_(output), parent_graph_(parent_graph), id_(0) { }

    bool Connection::input_output_type_compatible() const
    {
        return input_.accepts(output_.data_type()) || input_.accepts(ConnectionDataType::any());
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
        input_.remove_connection();
        output_.remove_connection(*this);
    }

    const Graph& Connection::parent_graph() const
    {
        return parent_graph_;
    }

    const ConnectionDataType& Connection::data_type() const
    {
        return output_.data_type();
    }

    int Connection::id() const
    {
        return id_;
    }

    void Connection::set_id(int id)
    {
        id_ = id;
    }
}
