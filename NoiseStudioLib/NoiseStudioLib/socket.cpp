#include "socket.h"

namespace noises
{
    Socket::Socket(const std::string& name, SocketType type)
        : name_(name), optional_(false), index_(0), connection_(), type_(type)
    {

    }

    std::string Socket::name() const
    {
        return name_;
    }

    void Socket::set_name(const std::string& name)
    {
        name_ = name;
    }

    bool Socket::optional() const
    {
        return optional_;
    }

    void Socket::set_optional(bool optional)
    {
        optional_ = optional;
    }

    Connection* Socket::connection() const
    {
        return connection_.get();
    }

    void Socket::set_connection(Connection *connection)
    {
        connection_.reset(connection);
    }

    void Socket::remove_connection()
    {
        connection_.reset();
    }

    unsigned int Socket::index() const
    {
        return index_;
    }

    void Socket::set_index(int index)
    {
        index_ = index;
    }

    SocketType Socket::type() const
    {
        return type_;
    }
}
