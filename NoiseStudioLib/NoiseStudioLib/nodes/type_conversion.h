#ifndef TYPE_CONVERSION_H
#define TYPE_CONVERSION_H

#include <graph_node.h>

namespace noises {
namespace nodes {

    enum class TypeConversionTargetType
    {
        int_t,
        long_t,
        float_t,
        double_t,
        uchar_t,
        uint_t
    };

    class TypeConversion : public GraphNode
    {
    public:
        TypeConversion();

        std::string node_name() const;

        void execute_uniforms(const CompositeDataBuffer& input, DataBuffer& output) const;
        void recalculate_sockets();

        void set_type(TypeConversionTargetType type);

    private:
        InputSocket* input_socket_;
        OutputSocket* output_socket_;
        Property* type_property_;

        const ConnectionDataType& get_type(TypeConversionTargetType target, int dimensions);
    };

}}



#endif // TYPE_CONVERSION_H
