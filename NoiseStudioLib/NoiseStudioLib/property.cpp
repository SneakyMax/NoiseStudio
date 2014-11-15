#include "property.h"

namespace noises
{
    Property::Property(const std::string &name, const ConnectionDataType &data_type)
        : name_(name),
          type_size_(data_type.size_full()),
          data_type_(data_type)
    {
        buffers_.resize(full_buffer_size());
    }

    const std::string& Property::name() const
    {
        return name_;
    }

    unsigned char* Property::get_buffer(int index) const
    {
        // Bleh...but whatever
        return const_cast<unsigned char*>(&buffers_[type_size() * index]);
    }

    std::size_t Property::type_size() const
    {
        return type_size_;
    }

    void Property::set_buffer(int index, const unsigned char *ptr)
    {
        std::copy(ptr, ptr + type_size(), get_buffer(index));
    }

    bool Property::has_value() const
    {
        return has_value_;
    }

    bool Property::has_default_value() const
    {
        return has_default_value_;
    }

    bool Property::has_min_value() const
    {
        return has_min_value_;
    }

    bool Property::has_max_value() const
    {
        return has_max_value_;
    }

    std::size_t Property::full_buffer_size() const
    {
        return type_size() * num_buffers();
    }

    void Property::listen_changed(std::function<void ()> handler)
    {
        listeners_.push_back(handler);
    }

    void Property::trigger_changed() const
    {
        for(std::function<void()> handler : listeners_)
        {
            handler();
        }
    }
}
