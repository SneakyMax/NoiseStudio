#ifndef SOCKET_H
#define SOCKET_H

#include <memory>
#include <functional>

#include "connection.h"
#include "socket_type.h"

namespace noises
{
    class GraphNode;

    class Socket
    {
    public:
        virtual ~Socket() { }

        std::string name() const;
        void set_name(const std::string& name);

        unsigned int index() const;
        void set_index(int index);

        SocketType type() const;

        Socket(const Socket&) = delete;
        Socket& operator=(const Socket&) = delete;

        GraphNode* parent();
        const GraphNode* parent() const;

        void set_parent(GraphNode* parent);

        void listen_changed(std::function<void()> handler);

    protected:
        Socket(const std::string& name, SocketType type);

        void trigger_changed() const;

    private:
        std::string name_;
        unsigned int index_;
        SocketType type_;
        GraphNode* parent_;

        std::vector<std::function<void()>> listeners_;
    };
}

#endif // SOCKET_H
