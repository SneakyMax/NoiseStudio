#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <vector>
#include <functional>

#include "socket_collection.h"
#include "data_buffer.h"
#include "input_socket.h"
#include "output_socket.h"
#include "property_collection.h"
#include "attrbute_info.h"

namespace noises
{
    class CompositeDataBuffer;
    class Property;
    class ValidationResults;

    class GraphNode
    {
    public:
        GraphNode();
        GraphNode(const GraphNode&) = delete;
        GraphNode& operator=(const GraphNode&) = delete;

        virtual ~GraphNode();

        /** Gets the node name which is the same for all nodes of the same type. E.g. "PerlinNoise" **/
        virtual std::string node_name() const = 0;

        /** Gets the name which   **/
        const std::string& name() const;
        void set_name(const std::string& name);

        std::string display_name() const;

        SocketCollection<InputSocket>& inputs();
        SocketCollection<OutputSocket>& outputs();

        const SocketCollection<InputSocket>& inputs() const;
        const SocketCollection<OutputSocket>& outputs() const;

        InputSocket& input(const std::string& name);
        const InputSocket& input(const std::string& name) const;

        OutputSocket& output(const std::string& name);
        const OutputSocket& output(const std::string &name) const;

        /** Executes a graph node and computes its uniform outputs. This method is executed once. **/
        virtual void execute_uniforms(const CompositeDataBuffer& input, DataBuffer& output) const;

        /** Executes a graph node and computes its attribute outputs. This method is called once for each index in the final output (e.g. x/y) **/
        virtual void execute_attributes(const CompositeDataBuffer& input, DataBuffer& output, DataBuffer::size_type index) const;

        virtual void validate(ValidationResults& results) const;

        int id() const;
        void set_id(int id);

        boost::optional<AttributeInfo> get_attribute_override() const;

        Property& property(const std::string& name);
        const Property& property(const std::string& name) const;

        const std::vector<std::reference_wrapper<Property>> properties();
        const std::vector<std::reference_wrapper<const Property>> properties() const;

        boost::optional<std::reference_wrapper<Property>> get_property_by_name(const std::string& name);
        boost::optional<std::reference_wrapper<const Property>> get_property_by_name(const std::string &name) const;

        void request_recalculate_sockets();

        // Don't really like these public here...but works for now
        bool is_graph_internal_node() const;
        void set_is_graph_internal_node(bool is_internal);

    protected:
        template<typename T, unsigned int Dimensions>
        Property& add_property(const std::string& name)
        {
            return properties_.add<T, Dimensions>(name);
        }

        void remove_property(const Property& property);

        /** Overrides the attribute length for a node. This will mean that execute_attributes will be called new_length times instead
         * of the length of the attributes passed into the node. Use this to, for example, change the size of an image.  **/
        void override_attribute_info(AttributeInfo info);

        /** Called when a connection is changed for a socket, a property is changed or added or removed, or the node is added to the graph.
         *  Use this to calculate socket output types (for example when they're dependent on the input type). **/
        virtual void recalculate_sockets();

    private:
        void recalculate_sockets_internal();

        PropertyCollection properties_;

        std::string name_;
        AttributeInfo attribute_override_;

        SocketCollection<InputSocket> inputs_;
        SocketCollection<OutputSocket> outputs_;

        int id_;
        bool in_recalculate_sockets_;
        bool is_graph_internal_node_;

    };
}

#endif // GRAPH_NODE_H
