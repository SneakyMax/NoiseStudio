#include "input_socket.h"

#include "graph.h"

namespace noises
{
    InputSocket::InputSocket(const std::string& name, SocketType type) :
        Socket(name, type), optional_(false), connection_(nullptr) { }

    void InputSocket::set_accepts(const ConnectionDataType& type)
    {
        accepted_types_.add(type);
        trigger_changed();
    }

    bool InputSocket::accepts(const ConnectionDataType& type) const
    {
        return accepted_types_.accepts(type);
    }

    const boost::optional<std::reference_wrapper<const Connection>> InputSocket::connection() const
    {
        if(connection_ == nullptr)
            return boost::none;
        return std::ref(*connection_);
    }

    void InputSocket::set_connection(const Connection& connection)
    {
        connection_ = &connection;
        trigger_changed();
    }

    void InputSocket::remove_connection()
    {
        connection_ = nullptr;
    }

    std::vector<std::reference_wrapper<const ConnectionDataType>> InputSocket::accepted_types() const
    {
        return accepted_types_.accepted_types();
    }

    bool InputSocket::optional() const
    {
        return optional_;
    }

    void InputSocket::set_optional(bool optional)
    {
        optional_ = optional;
        trigger_changed();
    }

    void InputSocket::on_removing()
    {
        Graph* graph = parent()->parent();
        if(graph == nullptr)
            return;

        graph->disconnect(*this);
    }
}
