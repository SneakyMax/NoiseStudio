#include "pixel_mapping.h"

#include <array>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

namespace noises {
namespace nodes {
namespace mappings {

    PixelMapping::PixelMapping()
    {
        inputs().add("Grid", SocketType::attribute);

        outputs().add("Mapped", ConnectionDataType::value<int, 2>(), SocketType::attribute);

        input("Grid").set_accepts(ConnectionDataType::any());

        output_ = &output("Mapped");
    }

    void PixelMapping::execute_uniforms(const CompositeDataBuffer&, DataBuffer &output) const
    {
        //TODO put this in utils
        //extracts the width and height out of the tag and puts them in the scratch buffer for execute_atttributes to use

        static const boost::regex finder("\\[([0-9]+),([0-9]+)\\]");

        std::string tag = output.attribute_info().tag();
        std::string::const_iterator start = tag.begin(), end = tag.end();
        boost::match_results<std::string::const_iterator> what;

        if(!boost::regex_search(start, end, what, finder, boost::match_default))
            throw std::logic_error("PixelMapping requires a tag to identify the data, or tag is missing [width,height]");

        int width = boost::lexical_cast<int>(static_cast<std::string>(what[1]));
        int height = boost::lexical_cast<int>(static_cast<std::string>(what[2]));

        std::array<int, 2> dimensions { width, height };
        output.set_scratch<std::array<int, 2>>(0, dimensions);
    }

    void PixelMapping::execute_attributes(const CompositeDataBuffer&, DataBuffer &output, DataBuffer::size_type index) const
    {
        std::array<int, 2> dimensions = output.get_scratch<std::array<int, 2>>(0);
        std::div_t row_column = std::div(index, dimensions[0]);
        std::array<int, 2> row_column_array { row_column.rem, row_column.quot };
        output.set_attribute<int, 2>(*output_, index, row_column_array);
    }

    std::string PixelMapping::node_name() const
    {
        return "Pixel Mapping";
    }

}}}
