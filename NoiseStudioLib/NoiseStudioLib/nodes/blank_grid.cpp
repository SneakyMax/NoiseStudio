#include "blank_grid.h"

#include <boost/format.hpp>

namespace noises {
namespace nodes
{
    BlankGrid::BlankGrid()
    {
        Property& width = add_property<unsigned int, 1>("Width");
        Property& height = add_property<unsigned int, 1>("Height");

        unsigned int default_width = 512;
        unsigned int default_height = 512;

        InputSocket& width_socket = inputs().add("Width", SocketType::uniform);
        InputSocket& height_socket = inputs().add("Height", SocketType::uniform);

        width_socket.set_accepts(ConnectionDataType::value<unsigned int, 1>());
        height_socket.set_accepts(ConnectionDataType::value<unsigned int, 1>());

        width.set_default_value<unsigned int, 1>(&default_width);
        height.set_default_value<unsigned int, 1>(&default_height);

        width_socket.set_optional(true);
        height_socket.set_optional(true);

        outputs().add("Grid", ConnectionDataType::undefined(), SocketType::attribute);
    }

    void BlankGrid::execute_uniforms(const CompositeDataBuffer&, DataBuffer &output) const
    {
        //TODO use input and output sockets if they're connected
        unsigned int width = property("Width").value_or_default<unsigned int, 1>().value();
        unsigned int height = property("Height").value_or_default<unsigned int, 1>().value();

        std::size_t length = width * height;

        // 2d image, data is rows, top to bottom, width/height
        std::string tag = boost::str(boost::format("[2d][w/h][t-b][%1%,%2%]") % width % height);

        output.resize_attribute(AttributeInfo(length, tag));
    }

    void BlankGrid::set_size(unsigned int width, unsigned int height)
    {
        property("Width").set_value<unsigned int>(&width);
        property("Height").set_value<unsigned int>(&height);
    }

    std::string BlankGrid::node_name() const
    {
        return "Grid";
    }
}}

