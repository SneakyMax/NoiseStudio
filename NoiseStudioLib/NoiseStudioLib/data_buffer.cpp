#include "data_buffer.h"

#include <cassert>
#include "socket_collection.h"

namespace noises
{
    DataBuffer::DataBuffer(size_type size) : attribute_size_(size) { }

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
        attribute_memory_blocks_.push_back(std::vector<unsigned char>(data_type.size_full() * attribute_size_));
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
        size_type real_index = should_equal.size_full() * index;
        return &(attribute_memory_blocks_[socket.index()][real_index]);
    }

    void DataBuffer::set_attribute_raw(const OutputSocket &socket, const ConnectionDataType& should_equal, size_type index, const unsigned char *value)
    {
        assert(socket.data_type() == should_equal);
        size_type real_index = should_equal.size_full() * index;
        unsigned char* ptr = &(attribute_memory_blocks_[socket.index()][real_index]);
        std::copy(value, value + should_equal.size_full(), ptr);
    }

    void DataBuffer::set_attribute_all_raw(const OutputSocket &socket, const ConnectionDataType &should_equal, const unsigned char *value, std::size_t length_check)
    {
        assert(socket.data_type() == should_equal);
        unsigned char* first_ptr = &(attribute_memory_blocks_[socket.index()][0]);
        std::size_t size = attribute_memory_blocks_[socket.index()].size();
        assert(size == length_check);
        std::copy(value, value + size, first_ptr);
    }

    const unsigned char* DataBuffer::get_uniform_raw(const OutputSocket &socket, const ConnectionDataType &should_equal) const
    {
        assert(socket.data_type() == should_equal);
        size_type real_index(get_uniform_index(socket.index()));
        return &(uniform_memory_block_[real_index]);
    }

    void DataBuffer::set_uniform_raw(const OutputSocket &socket, const ConnectionDataType &should_equal, const unsigned char *value)
    {
        assert(socket.data_type() == should_equal);
        size_type real_index(get_uniform_index(socket.index()));
        unsigned char* ptr = &(uniform_memory_block_[real_index]);
        std::copy(value, value + should_equal.size_full(), ptr);
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

    DataBuffer::size_type DataBuffer::attribute_size() const
    {
        return attribute_size_;
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
}
