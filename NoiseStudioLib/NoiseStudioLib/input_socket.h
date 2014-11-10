#ifndef INPUT_SOCKET_H
#define INPUT_SOCKET_H

#include "accepted_types.h"
#include "socket.h"

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
    private:
        AcceptedTypes accepted_types_;
    };
}


#endif // INPUT_SOCKET_H
