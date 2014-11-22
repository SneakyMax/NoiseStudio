#ifndef SOCKET_TYPE_H
#define SOCKET_TYPE_H

namespace noises
{
    enum class SocketType
    {
        /** Attributes vary for each position in the output data **/
        attribute,

        /** Uniforms are the same for each position in the output data **/
        uniform,

        /** For use on input sockets, when a socket will accept either uniform or attribute connections. **/
        either
    };
}

#endif // SOCKET_TYPE_H
