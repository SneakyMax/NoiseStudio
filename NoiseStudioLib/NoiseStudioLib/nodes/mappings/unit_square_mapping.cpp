#include "unit_square_mapping.h"

#include <array>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

namespace noises {
namespace nodes {
namespace mappings {

    UnitSquareMapping::UnitSquareMapping()
    {
        inputs().add("Grid", SocketType::attribute);

        outputs().add("Mapped", ConnectionDataType::value<float, 2>(), SocketType::attribute);

        input("Grid").set_accepts(ConnectionDataType::any());

        output_ = &output("Mapped");
    }

    std::string UnitSquareMapping::node_name() const
    {
        return "Unit Square Mapping";
    }

    void UnitSquareMapping::execute_uniforms(const CompositeDataBuffer&, DataBuffer &output) const
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

    void UnitSquareMapping::execute_attributes(const CompositeDataBuffer&, DataBuffer &output, DataBuffer::size_type index) const
    {
        std::array<int, 2> dimensions = output.get_scratch<std::array<int, 2>>(0);
        std::div_t row_column = std::div(index, dimensions[0]);

        float x = static_cast<float>(row_column.rem) / static_cast<float>(dimensions[0]);
        x = (x * 2) - 1; //-1 to 1

        float y = static_cast<float>(row_column.quot) / static_cast<float>(dimensions[1]);
        y = (y * 2) - 1;

        std::array<float, 2> x_y_array { x, y };

        output.set_attribute<float, 2>(*output_, index, x_y_array);
    }

}}}


