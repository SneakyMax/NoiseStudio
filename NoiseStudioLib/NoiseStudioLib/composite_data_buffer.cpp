#include "composite_data_buffer.h"

#include "input_socket.h"
#include <cassert>

namespace noises
{
    CompositeDataBuffer::CompositeDataBuffer(size_type attribute_size) : attribute_size_(attribute_size) { }

    void CompositeDataBuffer::add_attribute(const ConnectionDataType& data_type, const std::vector<unsigned char>& buffer)
    {
        attribute_data_types_.push_back(std::ref(data_type));
        attribute_refs_.push_back(std::ref(buffer));
    }

    void CompositeDataBuffer::add_uniform(const ConnectionDataType& data_type, const unsigned char* ptr)
    {
        size_type index = uniform_data_types_.size();

        uniform_data_types_.push_back(std::ref(data_type));
        uniform_memory_block_.resize(uniform_memory_block_.size() + data_type.size_full());

        size_type real_index(get_uniform_index(index));
        std::copy(ptr, ptr + data_type.size_full(), &uniform_memory_block_[real_index]);
    }

    const unsigned char* CompositeDataBuffer::get_attribute_internal(const InputSocket &socket, const ConnectionDataType &should_support, size_type index) const
    {
        assert(socket.accepts(should_support));
        assert(should_support == attribute_data_types_[socket.index()]);

        const std::reference_wrapper<const std::vector<unsigned char>>& buffer = attribute_refs_[socket.index()];
        size_type real_index(should_support.size_full() * index);

        return &(buffer.get()[real_index]);
    }

    const unsigned char* CompositeDataBuffer::get_uniform_internal(const InputSocket &socket, const ConnectionDataType &should_support) const
    {
        assert(socket.accepts(should_support));
        assert(should_support == uniform_data_types_[socket.index()]);

        size_type real_index(get_uniform_index(socket.index()));
        return &(uniform_memory_block_[real_index]);
    }

    CompositeDataBuffer::size_type CompositeDataBuffer::get_uniform_index(int index) const
    {
        size_type real_index(0);
        for(int i = 0; i < index; i++)
        {
            const ConnectionDataType& type = uniform_data_types_[i].get();
            real_index += type.size_full();
        }
        return real_index;
    }

    CompositeDataBuffer::size_type CompositeDataBuffer::attribute_size() const
    {
        return attribute_size_;
    }

    CompositeDataBuffer::size_type CompositeDataBuffer::num_attributes() const
    {
        return attribute_data_types_.size();
    }

    CompositeDataBuffer::size_type CompositeDataBuffer::num_uniforms() const
    {
        return uniform_data_types_.size();
    }

    CompositeDataBuffer::size_type CompositeDataBuffer::allocated_uniform_memory() const
    {
        return uniform_memory_block_.size();
    }

    const ConnectionDataType& CompositeDataBuffer::get_attribute_type(int index) const
    {
        return attribute_data_types_[index];
    }

    const ConnectionDataType& CompositeDataBuffer::get_uniform_type(int index) const
    {
        return uniform_data_types_[index];
    }
}
