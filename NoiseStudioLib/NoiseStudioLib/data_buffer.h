#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <cstddef>
#include <vector>
#include <memory>

#include "output_socket.h"

namespace noises
{
    template<typename Socket>
    class SocketCollection;

    /** Data buffer for the output of nodes. Use CompositeDataBuffer for inputs of nodes. */
    class DataBuffer
    {
    public:
        typedef std::vector<void*>::size_type size_type;

        DataBuffer(size_type attribute_size);

        void add(const SocketCollection<OutputSocket>& sockets);
        void add_attribute(const ConnectionDataType& data_type);
        void add_uniform(const ConnectionDataType& data_type);

        template<typename T, unsigned int Dimensions>
        const T* get_attribute(const OutputSocket& socket, size_type index) const
        {
            const unsigned char* ptr = get_attribute_internal(socket, ConnectionDataType::value<T, Dimensions>(), index);
            return reinterpret_cast<const T*>(ptr);
        }

        template<typename T, unsigned int Dimensions>
        void set_attribute(const OutputSocket& socket, size_type index, const T* value)
        {
            const unsigned char* ptr = reinterpret_cast<const unsigned char*>(value);
            set_attribute_internal(socket, ConnectionDataType::value<T, Dimensions>(), index, ptr);
        }

        template<typename T, unsigned int Dimensions>
        const T* get_uniform(const OutputSocket& socket) const
        {
            const unsigned char* ptr = get_uniform_internal(socket, ConnectionDataType::value<T, Dimensions>());
            return reinterpret_cast<const T*>(ptr);
        }

        template<typename T, unsigned int Dimensions>
        void set_uniform(const OutputSocket& socket, const T* value)
        {
            const unsigned char* ptr = reinterpret_cast<const unsigned char*>(value);
            set_uniform_internal(socket, ConnectionDataType::value<T, Dimensions>(), ptr);
        }

        size_type attribute_size() const;
        size_type num_attributes() const;
        size_type allocated_attribute_memory() const;

        size_type num_uniforms() const;
        size_type allocated_uniform_memory() const;

        const ConnectionDataType& get_attribute_type(int index) const;

    private:
        const unsigned char* get_attribute_internal(const OutputSocket& socket, const ConnectionDataType& should_equal, size_type index) const;
        void set_attribute_internal(const OutputSocket& socket, const ConnectionDataType& should_equal, size_type index, const unsigned char* value);

        const unsigned char* get_uniform_internal(const OutputSocket& socket, const ConnectionDataType& should_equal) const;
        void set_uniform_internal(const OutputSocket& socket, const ConnectionDataType& should_equal, const unsigned char* value);

        size_type get_uniform_index(int index) const;

        std::vector<std::reference_wrapper<const ConnectionDataType>> attribute_data_types_;
        std::vector<std::vector<unsigned char>> attribute_memory_blocks_;

        std::vector<std::reference_wrapper<const ConnectionDataType>> uniform_data_types_;
        std::vector<unsigned char> uniform_memory_block_;

        size_type attribute_size_;
    };
}


#endif // DATA_BUFFER_H
