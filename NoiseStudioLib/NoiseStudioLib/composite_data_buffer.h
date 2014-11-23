#ifndef COMPOSITE_DATA_BUFFER_H
#define COMPOSITE_DATA_BUFFER_H

#include <vector>
#include <tuple>

#include "ptr_array.h"
#include "connection_data_type.h"
#include "attribute_info.h"

namespace noises
{
    class InputSocket;

    class CompositeDataBuffer
    {
    public:
        typedef std::vector<void*>::size_type size_type;

        CompositeDataBuffer();
        CompositeDataBuffer(const CompositeDataBuffer&) = delete;
        CompositeDataBuffer& operator=(const CompositeDataBuffer&) = delete;

        void add_attribute(const ConnectionDataType& data_type, const std::vector<unsigned char>& buffer, AttributeInfo buffer_info);

        void add_uniform(const ConnectionDataType& data_type, const unsigned char* ptr);

        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> get_attribute(const InputSocket& socket, size_type index) const
        {
            return get_attribute_raw(socket, ConnectionDataType::value<T, Dimensions>(), index);
        }

        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> get_uniform(const InputSocket& socket) const
        {
            return get_uniform_raw(socket, ConnectionDataType::value<T, Dimensions>());
        }

        const unsigned char* get_attribute_raw(const InputSocket& socket, const ConnectionDataType& should_support, size_type index) const;
        std::tuple<const unsigned char*, std::size_t> get_attribute_all_raw(const InputSocket& socket, const ConnectionDataType& should_support) const;

        const unsigned char* get_uniform_raw(const InputSocket& socket, const ConnectionDataType& should_support) const;

        AttributeInfo attribute_info() const;

        size_type num_attributes() const;

        size_type num_uniforms() const;

        size_type allocated_uniform_memory() const;


        const ConnectionDataType& get_attribute_type(int index) const;

        const ConnectionDataType& get_uniform_type(int index) const;

    private:
        size_type get_uniform_index(int index) const;

        std::vector<std::reference_wrapper<const ConnectionDataType>> attribute_data_types_;
        std::vector<std::reference_wrapper<const ConnectionDataType>> uniform_data_types_;

        std::vector<std::reference_wrapper<const std::vector<unsigned char>>> attribute_refs_;
        std::vector<unsigned char> uniform_memory_block_;

        AttributeInfo attribute_info_;
    };
}



#endif // COMPOSITE_DATA_BUFFER_H
