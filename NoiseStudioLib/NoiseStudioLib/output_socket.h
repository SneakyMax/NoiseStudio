#ifndef OUTPUT_SOCKET_H
#define OUTPUT_SOCKET_H

#include <functional>
#include "connection_data_type.h"
#include "socket.h"

namespace noises
{
    class OutputSocket : public Socket
    {
    public:
        explicit OutputSocket(const std::string& name, const ConnectionDataType& data_type, SocketType type)
            : Socket(name, type), data_type_(data_type) { }

        const ConnectionDataType& data_type() const { return data_type_; }
        void set_data_type(const ConnectionDataType& data_type) { data_type_ = data_type; }

    private:
        std::reference_wrapper<const ConnectionDataType> data_type_;
    };
}

#endif // OUTPUT_SOCKET_H
