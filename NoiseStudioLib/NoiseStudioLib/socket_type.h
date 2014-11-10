#ifndef SOCKET_TYPE_H
#define SOCKET_TYPE_H

namespace noises
{
    enum class SocketType
    {
        /** Attributes vary for each position in the output data **/
        Attribute,

        /** Uniforms are the same for each posision in the output data **/
        Uniform
    };
}

#endif // SOCKET_TYPE_H
