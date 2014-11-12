#include "output_socket.h"

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
        auto it = connections_.begin();
        for(; it != connections_.end(); it++)
        {
            if(&((*it).get()) == &connection)
                break;
        }
        if(it != connections_.end())
            connections_.erase(it);
    }
}
