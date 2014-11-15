#ifndef SOCKET_COLLECTION_H
#define SOCKET_COLLECTION_H

#include <vector>
#include <memory>
#include <boost/optional.hpp>

#include "socket_type.h"
#include "connection_data_type.h"
#include "utils.h"

namespace noises
{
    class GraphNode;

    template<typename TSocket>
    class SocketCollection
    {
    public:
        SocketCollection(GraphNode* parent) : parent_(parent) { }
        SocketCollection(const SocketCollection&) = delete;
        SocketCollection& operator=(const SocketCollection&) = delete;

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
            socket_ref.set_parent(parent_);

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
            auto& sockets = socket->type() == SocketType::Uniform ? uniform_sockets_ : attribute_sockets_;

            sockets.erase(sockets.begin() + socket.index());

            // Update the indexes for all the sockets
            for(auto it = sockets.begin(), j = 0; it != sockets.end(); ++it, ++j)
            {
                it->set_index(j);
            }
        }

        boost::optional<std::reference_wrapper<TSocket>> get_by_name(const std::string& name)
        {
            auto const_value = static_cast<const SocketCollection<TSocket>*>(this)->get_by_name(name);
            if(!const_value)
                return boost::none;
            return std::ref(const_cast<TSocket&>(const_value->get()));
        }

        boost::optional<std::reference_wrapper<const TSocket>> get_by_name(const std::string &name) const
        {
            for(auto& ptr : attribute_sockets_)
            {
                if(ptr->name() == name)
                    return std::ref(static_cast<const TSocket&>(*ptr));
            }

            for(auto& ptr : uniform_sockets_)
            {
                if(ptr->name() == name)
                    return std::ref(static_cast<const TSocket&>(*ptr));
            }

            return boost::none;
        }

        TSocket& operator[](const std::string& name)
        {
            return const_cast<TSocket&>(static_cast<const SocketCollection<TSocket>*>(this)->operator[](name));
        }

        const TSocket& operator[](const std::string& name) const
        {
            auto socket = get_by_name(name);
            if(!socket)
                throw std::invalid_argument(name + " is not a valid socket.");
            return *socket;
        }

        std::vector<std::reference_wrapper<TSocket>> attribute_sockets()
        {
            return utils::to_reference_array(attribute_sockets_);
        }

        std::vector<std::reference_wrapper<TSocket>> uniform_sockets()
        {
            return utils::to_reference_array(uniform_sockets_);
        }

        const std::vector<std::reference_wrapper<const TSocket>> attribute_sockets() const
        {
            return utils::to_reference_array_const(attribute_sockets_);
        }

        const std::vector<std::reference_wrapper<const TSocket>> uniform_sockets() const
        {
            return utils::to_reference_array_const(uniform_sockets_);
        }

        std::vector<std::reference_wrapper<TSocket>> all_sockets()
        {
            return utils::concat(attribute_sockets(), uniform_sockets());
        }

        const std::vector<std::reference_wrapper<const TSocket>> all_sockets() const
        {
            return utils::concat(attribute_sockets(), uniform_sockets());
        }

        GraphNode* parent() { return parent_; }
        const GraphNode* parent() const { return parent_; }

    private:
        std::vector<std::unique_ptr<TSocket>> attribute_sockets_;
        std::vector<std::unique_ptr<TSocket>> uniform_sockets_;

        GraphNode* parent_;
    };
}



#endif // SOCKET_COLLECTION_H
