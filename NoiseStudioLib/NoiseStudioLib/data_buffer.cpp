#include "data_buffer.h"

#include <cassert>
#include "socket_collection.h"
#include "attribute_info.h"

namespace noises
{
    DataBuffer::DataBuffer(AttributeInfo attribute_info) : attribute_info_(attribute_info) { }

    void DataBuffer::add(const SocketCollection<OutputSocket> &sockets)
    {
        for(auto& socket : sockets.attribute_sockets())
        {
            add_attribute(socket.get().data_type());
        }

        for(auto& uniform_socket : sockets.uniform_sockets())
        {
            add_uniform(uniform_socket.get().data_type());
        }
    }

    void DataBuffer::add_attribute(const ConnectionDataType& data_type)
    {
        attribute_data_types_.push_back(std::ref(data_type));
        // reserve sizeof(T) * Dimensions * size_
        attribute_memory_blocks_.push_back(std::vector<unsigned char>(data_type.size_full() * attribute_info_.length()));
    }

    void DataBuffer::add_uniform(const ConnectionDataType& data_type)
    {
        uniform_data_types_.push_back(std::ref(data_type));

        //Allocate memory for the new uniform
        uniform_memory_block_.resize(uniform_memory_block_.size() + data_type.size_full());
    }

    const unsigned char* DataBuffer::get_attribute_raw(const OutputSocket& socket, const ConnectionDataType& should_equal, size_type index) const
    {
        assert(socket.data_type() == should_equal);
        return get_attribute_raw(socket.index(), index);
    }

    const unsigned char* DataBuffer::get_attribute_raw(unsigned int socket_index, size_type index) const
    {
        size_type real_index = get_attribute_type(socket_index).size_full() * index;
        return &(attribute_memory_blocks_[socket_index][real_index]);
    }

    void DataBuffer::set_attribute_raw(const OutputSocket &socket, const ConnectionDataType& should_equal, size_type index, const unsigned char *value)
    {
        assert(socket.data_type() == should_equal);
        set_attribute_raw(socket.index(), should_equal.size_full(), index, value);
    }

    void DataBuffer::set_attribute_raw(unsigned int socket_index, std::size_t value_size, size_type index, const unsigned char *value)
    {
        size_type real_index = value_size * index;
        unsigned char* ptr = &(attribute_memory_blocks_[socket_index][real_index]);
        std::copy(value, value + value_size, ptr);
    }

    void DataBuffer::set_attribute_all_raw(const OutputSocket &socket, const ConnectionDataType &should_equal, const unsigned char *value, std::size_t length_check)
    {
        assert(socket.data_type() == should_equal);
        set_attribute_all_raw(socket.index(), value, length_check);
    }

    void DataBuffer::set_attribute_all_raw(unsigned int socket_index, const unsigned char *value, std::size_t length_check)
    {
        unsigned char* first_ptr = &(attribute_memory_blocks_[socket_index][0]);
        std::size_t size = attribute_memory_blocks_[socket_index].size(); // Attribute memory block is initialized to the correct size
        assert(length_check == size);
        std::copy(value, value + size, first_ptr);
    }

    const unsigned char* DataBuffer::get_uniform_raw(const OutputSocket &socket, const ConnectionDataType &should_equal) const
    {
        assert(socket.data_type() == should_equal);
        return get_uniform_raw(socket.index());
    }

    const unsigned char* DataBuffer::get_uniform_raw(unsigned int index) const
    {
        size_type real_index(get_uniform_index(index));
        return &(uniform_memory_block_[real_index]);
    }

    void DataBuffer::set_uniform_raw(const OutputSocket &socket, const ConnectionDataType &should_equal, const unsigned char *value)
    {
        assert(socket.data_type() == should_equal);
        set_uniform_raw(socket.index(), should_equal.size_full(), value);
    }

    void DataBuffer::set_uniform_raw(unsigned int socket_index, std::size_t value_size, const unsigned char *value)
    {
        size_type real_index(get_uniform_index(socket_index));
        unsigned char* ptr = &(uniform_memory_block_[real_index]);
        std::copy(value, value + value_size, ptr);
    }

    DataBuffer::size_type DataBuffer::get_uniform_index(int index) const
    {
        size_type real_index(0);
        for(int i = 0; i < index; i++)
        {
            const ConnectionDataType& type = uniform_data_types_[i].get();
            real_index += type.size_full();
        }
        return real_index;
    }

    AttributeInfo DataBuffer::attribute_info() const
    {
        return attribute_info_;
    }

    DataBuffer::size_type DataBuffer::num_attributes() const
    {
        return attribute_data_types_.size();
    }

    DataBuffer::size_type DataBuffer::allocated_attribute_memory() const
    {
        size_type allocated_bytes(0);
        for(const std::vector<unsigned char>& block : attribute_memory_blocks_)
        {
            allocated_bytes += block.capacity();
        }
        return allocated_bytes;
    }

    const ConnectionDataType& DataBuffer::get_attribute_type(int index) const
    {
        return attribute_data_types_[index].get();
    }

    const ConnectionDataType& DataBuffer::get_uniform_type(int index) const
    {
        return uniform_data_types_[index].get();
    }

    DataBuffer::size_type DataBuffer::num_uniforms() const
    {
        return uniform_data_types_.size();
    }

    DataBuffer::size_type DataBuffer::allocated_uniform_memory() const
    {
        return uniform_memory_block_.size();
    }

    const std::vector<unsigned char>& DataBuffer::get_memory_block(int index) const
    {
        return attribute_memory_blocks_[index];
    }

    const unsigned char* DataBuffer::get_uniform_block(int index) const
    {
        return &(uniform_memory_block_[get_uniform_index(index)]);
    }

    void DataBuffer::resize_attribute(AttributeInfo info)
    {
        attribute_info_ = info;

        int i = 0;
        for(std::vector<unsigned char>& buffer : attribute_memory_blocks_)
        {
            buffer.resize(get_attribute_type(i).size_full() * info.length());
            i++;
        }
    }

    void DataBuffer::set_scratch_raw(unsigned int index, const ConnectionDataType &data_type, const unsigned char *value)
    {
        if(index >= scratch_blocks_.size())
            scratch_blocks_.resize(index + 1);

        std::vector<unsigned char>& buffer = scratch_blocks_.at(index);

        buffer.resize(data_type.size_full());
        std::copy(value, value + data_type.size_full(), buffer.data());
    }

    const unsigned char* DataBuffer::get_scratch_raw(unsigned int index, const ConnectionDataType &data_type) const
    {
        if(index >= scratch_blocks_.size())
            throw std::out_of_range("index is out of range");

        const std::vector<unsigned char>& buffer = scratch_blocks_.at(index);

        assert(buffer.size() == data_type.size_full());

        return buffer.data();
    }
}
