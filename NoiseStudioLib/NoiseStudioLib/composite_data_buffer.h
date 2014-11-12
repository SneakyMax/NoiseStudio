#ifndef COMPOSITE_DATA_BUFFER_H
#define COMPOSITE_DATA_BUFFER_H

#include <vector>
#include "connection_data_type.h"

namespace noises
{
    class InputSocket;

    class CompositeDataBuffer
    {
    public:
        typedef std::vector<void*>::size_type size_type;

        CompositeDataBuffer(size_type attribute_size);
        CompositeDataBuffer(const CompositeDataBuffer&) = delete;
        CompositeDataBuffer& operator=(const CompositeDataBuffer&) = delete;

        void add_attribute(const ConnectionDataType& data_type, const std::vector<unsigned char>& buffer);

        void add_uniform(const ConnectionDataType& data_type, const unsigned char* ptr);

        template<typename T, unsigned int Dimensions>
        const T* get_attribute(const InputSocket& socket, size_type index) const
        {
            const unsigned char* ptr = get_attribute_internal(socket, ConnectionDataType::value<T, Dimensions>(), index);
            return reinterpret_cast<const T*>(ptr);
        }

        template<typename T, unsigned int Dimensions>
        const T* get_uniform(const InputSocket& socket) const
        {
            const unsigned char* ptr = get_uniform_internal(socket, ConnectionDataType::value<T, Dimensions>());
            return reinterpret_cast<const T*>(ptr);
        }

        size_type attribute_size() const;

        size_type num_attributes() const;

        size_type num_uniforms() const;

        size_type allocated_uniform_memory() const;


        const ConnectionDataType& get_attribute_type(int index) const;

        const ConnectionDataType& get_uniform_type(int index) const;

    private:
        const unsigned char* get_attribute_internal(const InputSocket& socket, const ConnectionDataType& should_support, size_type index) const;
        const unsigned char* get_uniform_internal(const InputSocket& socket, const ConnectionDataType& should_support) const;

        size_type get_uniform_index(int index) const;

        std::vector<std::reference_wrapper<const ConnectionDataType>> attribute_data_types_;
        std::vector<std::reference_wrapper<const ConnectionDataType>> uniform_data_types_;

        std::vector<std::reference_wrapper<const std::vector<unsigned char>>> attribute_refs_;
        std::vector<unsigned char> uniform_memory_block_;

        size_type attribute_size_;
    };
}



#endif // COMPOSITE_DATA_BUFFER_H
