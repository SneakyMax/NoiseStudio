#ifndef SOCKET_COLLECTION_H
#define SOCKET_COLLECTION_H

#include <vector>
#include <memory>
#include "socket_type.h"

namespace noises
{
    template<typename TSocket>
    class SocketCollection
    {
    public:
        SocketCollection() {}
        SocketCollection(const SocketCollection&) = delete;

        void add(std::unique_ptr<TSocket> socket)
        {
            TSocket& socket_ref = *socket;

            if(socket_ref.type() == SocketType::Uniform)
            {
                uniform_sockets_.push_back(std::move(socket));
                socket_ref.set_index(uniform_sockets_.size() - 1);
            }
            else
            {
                attribute_sockets_.push_back(std::move(socket));
                socket_ref.set_index(attribute_sockets_.size() - 1);
            }
        }

        void remove(const TSocket* socket)
        {
            list_t& sockets = socket->type() == SocketType::Uniform ? uniform_sockets_ : attribute_sockets_;

            sockets.erase(sockets.begin() + socket.index());

            // Update the indexes for all the sockets
            for(auto it = sockets.begin(), j = 0; it != sockets.end(); ++it, ++j)
            {
                it->set_index(j);
            }
        }

        TSocket& get_by_name(const std::string& name)
        {
            for(auto it = attribute_sockets_.begin(); it != attribute_sockets_.end(); ++it)
            {
                if((*it)->name() == name)
                    return **it;
            }

            for(auto it = uniform_sockets_.begin(); it != attribute_sockets_.end(); ++it)
            {
                if((*it)->name() == name)
                    return **it;
            }

            throw std::logic_error("Could not find socket named " + name);
        }

        typedef std::vector<std::unique_ptr<TSocket>> list_t;

        list_t& attribute_sockets() { return attribute_sockets_; }
        list_t& uniform_sockets() { return uniform_sockets_; }

        const list_t& attribute_sockets() const { return attribute_sockets_; }
        const list_t& uniform_sockets() const { return uniform_sockets_; }

    private:
        list_t attribute_sockets_;
        list_t uniform_sockets_;
    };
}



#endif // SOCKET_COLLECTION_H
