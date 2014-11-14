#ifndef SOCKET_COLLECTION_H
#define SOCKET_COLLECTION_H

#include <vector>
#include <memory>
#include <boost/optional.hpp>

#include "socket_type.h"
#include "connection_data_type.h"

namespace noises
{
    template<typename TSocket>
    class SocketCollection
    {
    public:
        SocketCollection() {}
        SocketCollection(const SocketCollection&) = delete;

        TSocket& add(const std::string& name, SocketType type)
        {
            std::unique_ptr<TSocket> socket(new TSocket(name, type));
            TSocket& ref = *socket; // This works because the *unique_ptr* is moved, but the TSocket remains at the same memory location
            add(std::move(socket));
            return ref;
        }

        TSocket& add(const std::string& name, const ConnectionDataType& data_type, SocketType type)
        {
            std::unique_ptr<TSocket> socket(new TSocket(name, data_type, type));
            TSocket& ref = *socket;
            add(std::move(socket));
            return ref;
        }

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

        boost::optional<std::reference_wrapper<TSocket>> get_by_name(const std::string& name)
        {
            for(auto& ptr : attribute_sockets_)
            {
                if(ptr->name() == name)
                    return std::ref(*ptr);
            }

            for(auto& ptr : uniform_sockets_)
            {
                if(ptr->name() == name)
                    return std::ref(*ptr);
            }

            return boost::none;
        }

        TSocket& operator[](const std::string& name)
        {
            auto socket = get_by_name(name);
            if(!socket)
                throw std::invalid_argument(name + " is not a valid socket.");
            return socket.get().get();
        }

        TSocket& operator[](const char* name)
        {
            return (*this)[static_cast<std::string>(name)];
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
