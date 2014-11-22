#ifndef SOCKET_COLLECTION_H
#define SOCKET_COLLECTION_H

#include <vector>
#include <memory>
#include <boost/optional.hpp>

#include "socket_type.h"
#include "connection_data_type.h"

namespace noises
{
    class GraphNode;
    class Socket;

    template<typename TSocket>
    class SocketCollection
    {
    public:
        SocketCollection(GraphNode* parent) : parent_(parent) { }
        SocketCollection(const SocketCollection&) = delete;
        SocketCollection& operator=(const SocketCollection&) = delete;

        /** For adding input sockets. */
        TSocket& add(const std::string& name, SocketType type);
        /** For adding output sockets. */
        TSocket& add(const std::string& name, const ConnectionDataType& data_type, SocketType type);

        void add(std::unique_ptr<TSocket> socket);
        void remove(TSocket& socket);

        boost::optional<std::reference_wrapper<TSocket>> get_by_name(const std::string& name);

        boost::optional<std::reference_wrapper<const TSocket>> get_by_name(const std::string &name) const;

        TSocket& operator[](const std::string& name);

        const TSocket& operator[](const std::string& name) const;

        std::vector<std::reference_wrapper<TSocket>> attribute_sockets();
        std::vector<std::reference_wrapper<TSocket>> uniform_sockets();

        const std::vector<std::reference_wrapper<const TSocket>> attribute_sockets() const;
        const std::vector<std::reference_wrapper<const TSocket>> uniform_sockets() const;

        std::vector<std::reference_wrapper<TSocket>> all_sockets();
        const std::vector<std::reference_wrapper<const TSocket>> all_sockets() const;

        GraphNode* parent() { return parent_; }
        const GraphNode* parent() const { return parent_; }

        void listen_socket_changed(std::function<void(const TSocket&)> handler);

        void change_type(const std::string& name, SocketType target_type);

    private:
        void socket_changed(TSocket& socket);

        void try_place_input_either_socket(Socket& socket);
        void try_move_socket(Socket& socket, std::vector<std::unique_ptr<TSocket>>& from, std::vector<std::unique_ptr<TSocket>>& to);

        std::vector<std::unique_ptr<TSocket>> attribute_sockets_;
        std::vector<std::unique_ptr<TSocket>> uniform_sockets_;
        std::vector<std::unique_ptr<TSocket>> either_sockets_;

        GraphNode* parent_;

        std::vector<std::function<void(const TSocket&)>> listeners_;
    };
}

#include "utils.h"
#include "input_socket.h"
#include "output_socket.h"

namespace noises
{
    // Definitions

    template<typename TSocket>
    TSocket& SocketCollection<TSocket>::add(const std::string& name, SocketType type)
    {
        std::unique_ptr<TSocket> socket(new TSocket(name, type));
        TSocket& ref = *socket; // This works because the *unique_ptr* is moved, but the TSocket remains at the same memory location
        add(std::move(socket));
        return ref;
    }

    template<typename TSocket>
    TSocket& SocketCollection<TSocket>::add(const std::string& name, const ConnectionDataType& data_type, SocketType type)
    {
        std::unique_ptr<TSocket> socket(new TSocket(name, data_type, type));
        TSocket& ref = *socket;
        add(std::move(socket));
        return ref;
    }

    template<typename TSocket>
    void SocketCollection<TSocket>::add(std::unique_ptr<TSocket> socket)
    {
        TSocket& socket_ref = *socket;
        socket_ref.set_parent(parent_);

        if(socket_ref.type() == SocketType::uniform)
        {
            uniform_sockets_.push_back(std::move(socket));
            socket_ref.set_index(uniform_sockets_.size() - 1);
        }
        else if(socket_ref.type() == SocketType::attribute)
        {
            attribute_sockets_.push_back(std::move(socket));
            socket_ref.set_index(attribute_sockets_.size() - 1);
        }
        else if(socket_ref.type() == SocketType::either)
        {
            either_sockets_.push_back(std::move(socket));
            socket_ref.set_index(either_sockets_.size() - 1);
        }
        else throw std::logic_error("Unknown socket type");

        socket_ref.listen_changed([&socket_ref, this]() { socket_changed(socket_ref); });
    }

    template<typename TSocket>
    void SocketCollection<TSocket>::remove(TSocket& socket)
    {
        auto& sockets = socket.type() == SocketType::uniform ? uniform_sockets_ :
                        socket.type() == SocketType::attribute ? attribute_sockets_ :
                                                                  either_sockets_;

        socket.on_removing();
        sockets.erase(sockets.begin() + socket.index());

        // Update the indexes for all the sockets
        auto it = sockets.begin();
        int j = 0;
        for(; it != sockets.end(); ++it, ++j)
        {
            (*it)->set_index(j);
        }
    }

    template<typename TSocket>
    boost::optional<std::reference_wrapper<TSocket>> SocketCollection<TSocket>::get_by_name(const std::string& name)
    {
        auto const_value = static_cast<const SocketCollection<TSocket>*>(this)->get_by_name(name);
        if(!const_value)
            return boost::none;
        return std::ref(const_cast<TSocket&>(const_value->get()));
    }

    template<typename TSocket>
    boost::optional<std::reference_wrapper<const TSocket>> SocketCollection<TSocket>::get_by_name(const std::string &name) const
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

        for(auto& ptr : either_sockets_)
        {
            if(ptr->name() == name)
                return std::ref(static_cast<const TSocket&>(*ptr));
        }

        return boost::none;
    }

    template<typename TSocket>
    TSocket& SocketCollection<TSocket>::operator[](const std::string& name)
    {
        return const_cast<TSocket&>(static_cast<const SocketCollection<TSocket>*>(this)->operator[](name));
    }

    template<typename TSocket>
    const TSocket& SocketCollection<TSocket>::operator[](const std::string& name) const
    {
        auto socket = get_by_name(name);
        if(!socket)
            throw std::invalid_argument(name + " is not a valid socket.");
        return *socket;
    }

    template<typename TSocket>
    std::vector<std::reference_wrapper<TSocket>> SocketCollection<TSocket>::attribute_sockets()
    {
        return utils::to_reference_array(attribute_sockets_);
    }

    template<typename TSocket>
    std::vector<std::reference_wrapper<TSocket>> SocketCollection<TSocket>::uniform_sockets()
    {
        return utils::to_reference_array(uniform_sockets_);
    }

    template<typename TSocket>
    const std::vector<std::reference_wrapper<const TSocket>> SocketCollection<TSocket>::attribute_sockets() const
    {
        return utils::to_reference_array_const(attribute_sockets_);
    }

    template<typename TSocket>
    const std::vector<std::reference_wrapper<const TSocket>> SocketCollection<TSocket>::uniform_sockets() const
    {
        return utils::to_reference_array_const(uniform_sockets_);
    }

    template<typename TSocket>
    std::vector<std::reference_wrapper<TSocket>> SocketCollection<TSocket>::all_sockets()
    {
        return utils::concat(attribute_sockets(), uniform_sockets());
    }

    template<typename TSocket>
    const std::vector<std::reference_wrapper<const TSocket>> SocketCollection<TSocket>::all_sockets() const
    {
        return utils::concat(attribute_sockets(), uniform_sockets());
    }

    template<typename TSocket>
    void SocketCollection<TSocket>::listen_socket_changed(std::function<void (const TSocket &)> handler)
    {
        listeners_.push_back(handler);
    }

    template<typename TSocket>
    void SocketCollection<TSocket>::socket_changed(TSocket &socket)
    {
        try_place_input_either_socket(socket);

        for(std::function<void(const TSocket&)> handler : listeners_)
        {
            handler(socket);
        }
    }

    template<typename TSocket>
    void SocketCollection<TSocket>::try_place_input_either_socket(Socket& socket)
    {
        if(socket.type() != SocketType::either)
            return;

        // Try to place the either socket
        InputSocket* input_socket = dynamic_cast<InputSocket*>(&socket);
        if(input_socket != nullptr)
        {
            auto possible_connection = input_socket->connection();
            if(possible_connection)
            {
                const Connection& connection = *possible_connection;
                if(connection.output().type() == SocketType::attribute)
                {
                    try_move_socket(socket, uniform_sockets_, attribute_sockets_);
                    try_move_socket(socket, either_sockets_, attribute_sockets_);
                }
                else
                {
                    try_move_socket(socket, attribute_sockets_, uniform_sockets_);
                    try_move_socket(socket, either_sockets_, uniform_sockets_);
                }
            }
            else
            {
                try_move_socket(socket, attribute_sockets_, either_sockets_);
                try_move_socket(socket, uniform_sockets_, either_sockets_);
            }
        }
        else
        {
            throw std::logic_error("Either sockets must be input sockets!");
        }
    }
    template<typename TSocket>
    void SocketCollection<TSocket>::try_move_socket(Socket& socket, std::vector<std::unique_ptr<TSocket>>& from, std::vector<std::unique_ptr<TSocket>>& to)
    {
        auto it = std::find_if(from.begin(), from.end(), [&socket](std::unique_ptr<TSocket>& socket_ptr) { return &socket == socket_ptr.get(); });
        if(it == from.end())
            return;

        std::unique_ptr<TSocket> new_ptr;
        std::swap(new_ptr, *it);
        from.erase(it);
        to.push_back(std::move(new_ptr));
        socket.set_index(to.size() - 1);
    }

    template<typename TSocket>
    void SocketCollection<TSocket>::change_type(const std::string& name, SocketType target_type)
    {
        TSocket& socket = operator[](name);

        if(target_type == SocketType::attribute)
        {
            try_move_socket(socket, uniform_sockets_, attribute_sockets_);
            try_move_socket(socket, either_sockets_, attribute_sockets_);
        }
        else if(target_type == SocketType::uniform)
        {
            try_move_socket(socket, attribute_sockets_, uniform_sockets_);
            try_move_socket(socket, either_sockets_, uniform_sockets_);
        }
        else if(target_type == SocketType::either)
        {
            try_move_socket(socket, uniform_sockets_, either_sockets_);
            try_move_socket(socket, attribute_sockets_, either_sockets_);
        }
        else throw std::logic_error("Unknown socket type");

        socket.set_type(target_type);

        if(target_type == SocketType::either)
            try_place_input_either_socket(socket);
    }
}





#endif // SOCKET_COLLECTION_H
