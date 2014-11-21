#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <cstddef>
#include <vector>
#include <memory>

#include "output_socket.h"
#include "ptr_array.h"

namespace noises
{
    template<typename Socket>
    class SocketCollection;

    /** Data buffer for the output of nodes. Use CompositeDataBuffer for inputs of nodes. */
    class DataBuffer
    {
    public:
        typedef std::vector<void*>::size_type size_type;

        /** Creates a new data buffer, specifying attribute_size for the size of attribute types (e.g. position as one number) **/
        DataBuffer(size_type attribute_size);
        DataBuffer(const DataBuffer&) = delete;
        DataBuffer& operator=(const DataBuffer&) = delete;


        /** Adds a socket collection's attributes and uniforms to the data buffer. This is the preferred way to add data types.  **/
        void add(const SocketCollection<OutputSocket>& sockets);

        /** Adds an attribute data type to the data buffer. The index is the last index + 1. **/
        void add_attribute(const ConnectionDataType& data_type);

        /** Adds a uniform data type to the data buffer. The index is the last index + 1 **/
        void add_uniform(const ConnectionDataType& data_type);


        /** Gets an attribute value of a socket. {index} is the attribute index.  **/
        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> get_attribute(const OutputSocket& socket, size_type index) const
        {
            return get_attribute_raw(socket, ConnectionDataType::value<T, Dimensions>(), index);
        }

        /** Sets an attribute value of a socket. {index} is the attribute index. {value} must be an array of {Dimensions} size. **/
        template<typename T, unsigned int Dimensions>
        void set_attribute(const OutputSocket& socket, size_type index, const ptr_array<T, Dimensions> ptr)
        {
            set_attribute_raw(socket, ConnectionDataType::value<T, Dimensions>(), index, ptr.raw());
        }

        /** Gets the value of the uniform for the socket. **/
        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> get_uniform(const OutputSocket& socket) const
        {
            return get_uniform_raw(socket, ConnectionDataType::value<T, Dimensions>());
        }

        /** Sets the value of the uniform for the socket. {value} must be an array of {Dimensions} size. **/
        template<typename T, unsigned int Dimensions>
        void set_uniform(const OutputSocket& socket, const ptr_array<T, Dimensions> ptr)
        {
            set_uniform_raw(socket, ConnectionDataType::value<T, Dimensions>(), ptr.raw());
        }


        /** Gets the size of the attributes (the value passed into the constructor) **/
        size_type attribute_size() const;

        /** Gets the number of attributes added to the data buffer. **/
        size_type num_attributes() const;

        /** Gets the number of allocated bytes for all buffers of attributes. **/
        size_type allocated_attribute_memory() const;


        /** Gets the number of uniforms added to the data buffer. **/
        size_type num_uniforms() const;

        /** Gets the number of allocated bytes for all uniforms. **/
        size_type allocated_uniform_memory() const;


        /** Gets the ConnectionDataType for the attribute with the specified index. **/
        const ConnectionDataType& get_attribute_type(int index) const;

        /** Gets the ConnectionDataType for the uniform with the specified index. **/
        const ConnectionDataType& get_uniform_type(int index) const;


        // Below are methods to interact with the raw data. Only use these if you have to. Prefer the templated methods.

        /** Gets a reference to a memory block for the attribute with the specified index. Used by a Connection to create a CompositeDataBuffer. **/
        const std::vector<unsigned char>& get_memory_block(int index) const;

        /** Gets a pointer to the first member of a data block containing uniform data. Used by a Connection to create a CompositeDataBuffer. **/
        const unsigned char* get_uniform_block(int index) const;

        /** Gets a single attribute value as a raw byte array. Use this for library code that doesn't know the template parameters. */
        const unsigned char* get_attribute_raw(const OutputSocket& socket, const ConnectionDataType& should_equal, size_type index) const;
        /** Gets a single attribute value as a raw byte array, with *no* type checking. only use this if you're sure you're messing with the right data type (dangerous). */
        const unsigned char* get_attribute_raw(unsigned int socket_index, size_type index) const;

        /** Sets an attribute value as a raw byte array. Use this for library code that doesn't know the template parameters. */
        void set_attribute_raw(const OutputSocket& socket, const ConnectionDataType& should_equal, size_type index, const unsigned char* value);
        /** Sets an attribute value as a raw byte array, with *no* type checking. Only use this if you're sure you're messing with the right data type (dangerous). */
        void set_attribute_raw(unsigned int socket_index, std::size_t value_size, size_type index, const unsigned char* value);

        /** Sets the entire attribute using a raw byte array. Use this for library code that doesn't know the template parameters. */
        void set_attribute_all_raw(const OutputSocket& socket, const ConnectionDataType& should_equal, const unsigned char* value, std::size_t length_check);
        /** Sets the entire attribute using a raw byte array, with *no* type checking. Only use this if you're sure you're messing with the right data type (dangerous) */
        void set_attribute_all_raw(unsigned int socket_index, const unsigned char* value, std::size_t length_check);


        /** Gets a uniform as a raw byte array. Use this for library code that doesn't know the template parameters. */
        const unsigned char* get_uniform_raw(const OutputSocket& socket, const ConnectionDataType& should_equal) const;
        /** Gets a uniform as a raw byte array, with *no* type checking. Only use this if you're sure you're messing with the right data type (dangerous). */
        const unsigned char* get_uniform_raw(unsigned int index) const;

        /** Sets a uniform as a raw byte array. Use this for library code that doesn't know the template parameters. */
        void set_uniform_raw(const OutputSocket& socket, const ConnectionDataType& should_equal, const unsigned char* value);
        /** Sets a uniform as a raw byte array, with *no* type checking. Only use this if you're sure you're messing with the right data type (dangerous). */
        void set_uniform_raw(unsigned int socket_index, std::size_t value_size, const unsigned char* value);

    private:
        size_type get_uniform_index(int index) const;

        std::vector<std::reference_wrapper<const ConnectionDataType>> attribute_data_types_;
        std::vector<std::vector<unsigned char>> attribute_memory_blocks_;

        std::vector<std::reference_wrapper<const ConnectionDataType>> uniform_data_types_;
        std::vector<unsigned char> uniform_memory_block_;

        size_type attribute_size_;
    };
}


#endif // DATA_BUFFER_H
