#ifndef ATTRIBUTE_INFO_H
#define ATTRIBUTE_INFO_H

#include <cstddef>
#include <string>

namespace noises
{
    struct AttributeInfo
    {
    public:
        AttributeInfo() : AttributeInfo(0, "") { }

        AttributeInfo(std::size_t length) : AttributeInfo(length, "") { }

        AttributeInfo(std::size_t length, const std::string& tag) :
            length_(length), tag_(tag) { }

        std::size_t length() const { return length_; }

        /** A tag will contain something like "image512x512" so that you can identify what the data contains **/
        const std::string& tag() const { return tag_; }

        static const AttributeInfo inherit() { return AttributeInfo(0, ""); }
    private:
        std::size_t length_;
        std::string tag_;
    };

    inline bool operator==(const AttributeInfo& l, const AttributeInfo& r)
    {
        //Should check tag too?
        return l.length() == r.length();
    }
}



#endif // ATTRIBUTE_INFO_H
