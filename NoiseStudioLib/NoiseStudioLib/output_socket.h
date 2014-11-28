#ifndef OUTPUT_SOCKET_H
#define OUTPUT_SOCKET_H

#include <vector>
#include <functional>
#include "connection_data_type.h"
#include "socket.h"

namespace noises
{
    class OutputSocket : public Socket
    {
    public:
        OutputSocket(const std::string& name, const ConnectionDataType& data_type, SocketType type);

        const ConnectionDataType& data_type() const;
        void set_data_type(const ConnectionDataType& data_type);

        const std::vector<std::reference_wrapper<const Connection>>& connections() const;

        void add_connection(const Connection& connection);
        void remove_connection(const Connection& connection);
        void remove_all_connections();

        void on_removing();

    private:
        std::reference_wrapper<const ConnectionDataType> data_type_;
        std::vector<std::reference_wrapper<const Connection>> connections_;
    };
}

#endif // OUTPUT_SOCKET_H
