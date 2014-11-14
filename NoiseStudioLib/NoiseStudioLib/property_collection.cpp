#include "property_collection.h"

#include <stdexcept>

#include "utils.h"

namespace noises
{
    PropertyCollection::PropertyCollection() { }

    void PropertyCollection::remove(const std::string &name)
    {
        auto property = get_property_by_name(name);
        if(property)
            utils::remove_by_pointer(properties_, &property->get());
    }

    boost::optional<std::reference_wrapper<Property>> PropertyCollection::get_property_by_name(const std::string& name)
    {
        auto it = std::find_if(properties_.begin(), properties_.end(), [name](std::unique_ptr<Property>& ptr) { return ptr->name() == name; });
        if(it == properties_.end())
            return boost::none;
        return std::ref(**it);
    }

    boost::optional<std::reference_wrapper<const Property>> PropertyCollection::get_property_by_name(const std::string& name) const
    {
        auto it = std::find_if(properties_.begin(), properties_.end(), [name](const std::unique_ptr<Property>& ptr) { return ptr->name() == name; });
        if(it == properties_.end())
            return boost::none;
        const Property& prop = **it;
        return std::ref(prop);
    }

    Property& PropertyCollection::operator[](const std::string& name)
    {
        return const_cast<Property&>(const_cast<const PropertyCollection*>(this)->operator[](name));
    }

    const Property& PropertyCollection::operator[](const std::string& name) const
    {
        auto property = get_property_by_name(name);
        if(!property)
            throw std::invalid_argument(name + " is an invalid property name.");
        return *property;
    }

    std::vector<std::reference_wrapper<Property>> PropertyCollection::properties()
    {
        std::vector<std::reference_wrapper<Property>> return_props;
        for(auto& prop : properties_)
        {
            return_props.push_back(std::ref(*prop));
        }
        return return_props;
    }

    const std::vector<std::reference_wrapper<const Property>> PropertyCollection::properties() const
    {
        std::vector<std::reference_wrapper<const Property>> return_props;
        for(auto& prop : properties_)
        {
            return_props.push_back(std::ref(static_cast<const Property&>(*prop)));
        }
        return return_props;
    }

    bool PropertyCollection::has_property(const std::string &name) const
    {
        return static_cast<bool>(get_property_by_name(name));
    }

    std::size_t PropertyCollection::count() const
    {
        return properties_.size();
    }
}
