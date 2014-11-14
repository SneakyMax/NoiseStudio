#include "output_socket.h"
#include "utils.h"

namespace noises
{
    OutputSocket::OutputSocket(const std::string &name, const ConnectionDataType &data_type, SocketType type)
                : Socket(name, type), data_type_(data_type) { }

    const ConnectionDataType& OutputSocket::data_type() const
    {
        return data_type_;
    }

    void OutputSocket::set_data_type(const ConnectionDataType& data_type)
    {
        data_type_ = data_type;
    }

    const std::vector<std::reference_wrapper<const Connection>>& OutputSocket::connections() const
    {
        return connections_;
    }

    void OutputSocket::add_connection(const Connection& connection)
    {
        remove_connection(connection);
        connections_.push_back(std::ref(connection));
    }

    void OutputSocket::remove_connection(const Connection& connection)
    {
        utils::remove_by_pointer(connections_, const_cast<Connection*>(&connection));
    }
}
