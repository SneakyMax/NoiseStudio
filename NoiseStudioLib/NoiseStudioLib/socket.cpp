#include "socket.h"

namespace noises
{
    Socket::Socket(const std::string& name, SocketType type)
        : name_(name), index_(0), type_(type), parent_(nullptr)
    {

    }

    std::string Socket::name() const
    {
        return name_;
    }

    void Socket::set_name(const std::string& name)
    {
        trigger_changed();
        name_ = name;
    }

    unsigned int Socket::index() const
    {
        return index_;
    }

    void Socket::set_index(int index)
    {
        trigger_changed();
        index_ = index;
    }

    SocketType Socket::type() const
    {
        return type_;
    }

    GraphNode* Socket::parent()
    {
        return parent_;
    }

    const GraphNode* Socket::parent() const
    {
        return parent_;
    }

    void Socket::set_parent(GraphNode *parent)
    {
        parent_ = parent;
        trigger_changed();
    }

    void Socket::trigger_changed() const
    {
        for(std::function<void()> handler : listeners_)
        {
            handler();
        }
    }

    void Socket::listen_changed(std::function<void ()> handler)
    {
        listeners_.push_back(handler);
    }

    void Socket::set_type(SocketType type)
    {
        type_ = type;
        trigger_changed();
    }
}
