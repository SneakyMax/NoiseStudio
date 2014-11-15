#ifndef INPUT_SOCKET_H
#define INPUT_SOCKET_H

#include <boost/optional.hpp>

#include "accepted_types.h"
#include "socket.h"
#include "connection.h"

namespace noises
{
    class InputSocket : public Socket
    {
    public:
        InputSocket(const std::string& name, SocketType type);
        InputSocket(const InputSocket&) = delete;
        InputSocket operator=(const InputSocket&) = delete;

        void set_accepts(const ConnectionDataType& type);
        bool accepts(const ConnectionDataType& type) const;
        std::vector<std::reference_wrapper<const ConnectionDataType>> accepted_types() const;

        bool optional() const;
        void set_optional(bool optional);

        const boost::optional<std::reference_wrapper<const Connection>> connection() const;
        void set_connection(const Connection& connection);
        void remove_connection();

    private:
        AcceptedTypes accepted_types_;
        const Connection* connection_;
        bool optional_;
    };
}


#endif // INPUT_SOCKET_H
