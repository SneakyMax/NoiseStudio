#include "composite_data_buffer.h"

#include <cassert>

#include "input_socket.h"


namespace noises
{
    CompositeDataBuffer::CompositeDataBuffer() : attribute_info_(0, "") { }

    void CompositeDataBuffer::add_attribute(const ConnectionDataType& data_type, const std::vector<unsigned char>& buffer, AttributeInfo buffer_info)
    {
        if(attribute_info_.length() == 0)
        {
            attribute_info_ = buffer_info;
        }
        else
        {
            if(buffer_info.length() != attribute_info_.length())
            {
                throw std::invalid_argument("Attributes are not same length.");
            }
        }

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

    const unsigned char* CompositeDataBuffer::get_attribute_raw(const InputSocket &socket, const ConnectionDataType &should_support, size_type index) const
    {
        assert(socket.accepts(should_support));
        assert(should_support == attribute_data_types_[socket.index()]);

        const std::reference_wrapper<const std::vector<unsigned char>>& buffer = attribute_refs_[socket.index()];
        size_type real_index(should_support.size_full() * index);

        return &(buffer.get()[real_index]);
    }

    std::tuple<const unsigned char*, std::size_t> CompositeDataBuffer::get_attribute_all_raw(const InputSocket &socket, const ConnectionDataType &should_support) const
    {
        assert(socket.accepts(should_support));
        assert(should_support == attribute_data_types_[socket.index()]);

        const std::reference_wrapper<const std::vector<unsigned char>>& buffer = attribute_refs_[socket.index()];
        const unsigned char* first = &(buffer.get()[0]);
        const std::size_t size = buffer.get().size();
        return std::make_tuple(first, size);
    }

    const unsigned char* CompositeDataBuffer::get_uniform_raw(const InputSocket &socket, const ConnectionDataType &should_support) const
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

    AttributeInfo CompositeDataBuffer::attribute_info() const
    {
        return attribute_info_;
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
