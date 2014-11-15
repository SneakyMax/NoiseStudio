#ifndef PROPERTY_COLLECTION_H
#define PROPERTY_COLLECTION_H

#include <boost/optional.hpp>
#include <memory>

#include "property.h"

namespace noises
{
    class PropertyCollection
    {
    public:
        PropertyCollection();
        PropertyCollection(const PropertyCollection&) = delete;
        PropertyCollection& operator=(const PropertyCollection&) = delete;
        PropertyCollection(PropertyCollection&&) = default;

        /** Adds a property to the collection. Throws an exception if the property is not part of the collection. **/
        template<typename T, unsigned int Dimensions>
        Property& add(const std::string& name)
        {
            if(has_property(name))
                throw std::logic_error(name + " has already been taken in the property collection.");
            std::unique_ptr<Property> property(new Property(name, ConnectionDataType::value<T, Dimensions>()));
            Property& ref = *property;
            properties_.push_back(std::move(property));
            ref.listen_changed([this, &ref]() { trigger_changed(ref); });
            trigger_changed(ref);
            return ref;
        }

        /** Removes a property from the collection. Does nothing if the property is not part of the collection. **/
        void remove(const std::string& name);

        /** Gets a property by name. Returns a false boost optional if the property is not part of the collection. **/
        boost::optional<std::reference_wrapper<Property>> get_property_by_name(const std::string& name);
        boost::optional<std::reference_wrapper<const Property>> get_property_by_name(const std::string &name) const;

        /** Gets a property by name. Throws an exception if the property is not part of the collection. **/
        Property& operator[](const std::string& name);
        const Property& operator[](const std::string& name) const;

        /** Gets all properties in the collection. **/
        std::vector<std::reference_wrapper<Property>> properties();
        const std::vector<std::reference_wrapper<const Property>> properties() const;

        /** Returns whether the collection has a property with the specified name. **/
        bool has_property(const std::string& name) const;

        /** Returns the number of properties in the collection. **/
        std::size_t count() const;

        void listen_changed(std::function<void(Property&)> handler);

    private:
        void trigger_changed(Property& property) const;

        std::vector<std::unique_ptr<Property>> properties_;
        std::vector<std::function<void(Property&)>> listeners_;
    };
}

#endif // PROPERTY_COLLECTION_H
