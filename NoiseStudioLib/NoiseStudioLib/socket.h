#ifndef SOCKET_H
#define SOCKET_H

#include <memory>
#include "connection.h"
#include "socket_type.h"

namespace noises
{
    class Socket
    {
    public:
        virtual ~Socket() { }

        std::string name() const;
        void set_name(const std::string& name);

        bool optional() const;
        void set_optional(bool optional);

        Connection* connection() const;
        void set_connection(Connection* connection);
        void remove_connection();

        unsigned int index() const;
        void set_index(int index);

        SocketType type() const;

    protected:
        Socket(const std::string& name, SocketType type);

    private:
        std::string name_;
        bool optional_;
        unsigned int index_;
        SocketType type_;

        std::shared_ptr<Connection> connection_;
    };
}

#endif // SOCKET_H
