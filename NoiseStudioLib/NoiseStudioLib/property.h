#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <vector>
#include <array>

#include "connection_data_type.h"
#include "ptr_array.h"

namespace noises
{
    class Property
    {
    public:
        enum class buffers { value = 0, default_value, min_value, max_value };

        Property(const std::string& name, const ConnectionDataType& data_type);
        Property(const Property&) = delete;
        Property& operator=(const Property&) = delete;

        Property(Property&&) = default;

        const std::string& name() const;

        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> value() const
        {
            check<T, Dimensions>();
            return reinterpret_cast<T*>(get_buffer(buffers::value));
        }

        template<typename T, unsigned int Dimensions>
        void set_value(const ptr_array<T, Dimensions> value)
        {
            check<T, Dimensions>();
            set_buffer(buffers::value, value.raw());
            has_value_ = true;
        }

        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> default_value() const
        {
            check<T, Dimensions>();
            return get_buffer(buffers::default_value);
        }

        template<typename T, unsigned int Dimensions>
        void set_default_value(const ptr_array<T, Dimensions> value)
        {
            check<T, Dimensions>();
            set_buffer(buffers::default_value, value.raw());
            has_default_value_ = true;
        }

        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> min_value() const
        {
            check<T, Dimensions>();
            return reinterpret_cast<T*>(get_buffer(buffers::min_value));
        }

        template<typename T, unsigned int Dimensions>
        void set_min_value(const ptr_array<T, Dimensions> value)
        {
            check<T, Dimensions>();
            set_buffer(buffers::min_value, value.raw());
            has_min_value_ = true;
        }

        template<typename T, unsigned int Dimensions>
        const ptr_array<T, Dimensions> max_value() const
        {
            check<T, Dimensions>();
            return reinterpret_cast<T*>(get_buffer(buffers::max_value));
        }

        template<typename T, unsigned int Dimensions>
        void set_max_value(const ptr_array<T, Dimensions> value)
        {
            check<T, Dimensions>();
            set_buffer(buffers::max_value, value.raw());
            has_max_value_ = true;
        }

        bool has_value() const;
        bool has_default_value() const;
        bool has_min_value() const;
        bool has_max_value() const;


        std::size_t type_size() const;

        std::size_t full_buffer_size() const;

        std::size_t num_buffers() const { return 4; }

    private:
        template<typename T, unsigned int Dimensions>
        void check()
        {
            auto& data_type = ConnectionDataType::value<T, Dimensions>();
            assert(data_type == data_type_);
        }

        inline unsigned char* get_buffer(buffers buffer) const { return get_buffer((int)buffer); }
        unsigned char* get_buffer(int index) const;

        inline void set_buffer(buffers buffer, unsigned char* ptr) { set_buffer((int)buffer, ptr); }
        void set_buffer(int index, unsigned char* ptr);

        std::vector<unsigned char> buffers_;

        const std::string name_;
        const std::size_t type_size_;
        const ConnectionDataType& data_type_;

        bool has_value_;
        bool has_default_value_;
        bool has_min_value_;
        bool has_max_value_;
    };
}



#endif // PROPERTY_H
