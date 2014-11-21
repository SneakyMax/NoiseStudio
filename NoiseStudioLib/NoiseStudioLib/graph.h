#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <memory>
#include <boost/optional.hpp>
#include <utility>
#include <unordered_map>

#include "graph_node.h"
#include "nodes/attribute_buffer.h"
#include "nodes/uniform_buffer.h"

namespace noises
{
    class GraphOutputs;
    class Graph
    {
    public:
        Graph();

        /** Simple way to add a node to the graph. **/
        template<typename T, typename... Args>
        T& add_node(Args&&... args)
        {
            std::unique_ptr<T> node(new T(std::forward(args)... ));
            T& node_ref = *node;
            add_node(std::move(node));
            return node_ref;
        }

        /** Adds a graph node to the graph. Returns the id of the added node. **/
        int add_node(std::unique_ptr<GraphNode> node);

        /** Removes a graph node from the graph. The passed in reference is invalidated (the node is destroyed) **/
        void remove_node(GraphNode* node);


        /** Gets a list of graph nodes by their node name (same among node types) **/
        std::vector<GraphNode*> get_nodes_by_node_name(const std::string& name) const;

        /** Gets a graph node by its unique name. **/
        boost::optional<std::reference_wrapper<GraphNode>> get_node_by_name(const std::string& name) const;

        /** Gets a graph node by its id. **/
        boost::optional<std::reference_wrapper<GraphNode>> get_node_by_id(int id) const;


        /** Connects the output sockets of one graph node to the input sockets of another graph node. **/
        void connect(OutputSocket& output, InputSocket& input);

        /** Disconnects the connection for an input socket. **/
        void disconnect(InputSocket& input);

        /** Disconnects all connections coming out of an output socket. **/
        void disconnect_all(OutputSocket& output);

        /** Gets all connections in the graph. **/
        std::vector<std::reference_wrapper<Connection>> connections();
        const std::vector<std::reference_wrapper<const Connection>> connections() const;

        std::vector<std::reference_wrapper<GraphNode>> nodes();
        const std::vector<std::reference_wrapper<const GraphNode>> nodes() const;

        template<typename T, unsigned int Dimensions>
        Property& add_property(const std::string& name)
        {
            return properties_.add<T, Dimensions>(name);
        }

        void remove_property(const std::string& name);
        boost::optional<std::reference_wrapper<Property>> get_property_by_name(const std::string& name);

        boost::optional<std::reference_wrapper<nodes::AttributeBuffer>> get_attribute_input(const std::string& name);
        boost::optional<std::reference_wrapper<nodes::AttributeBuffer>> get_attribute_output(const std::string& name);

        boost::optional<std::reference_wrapper<nodes::UniformBuffer>> get_uniform_input(const std::string& name);
        boost::optional<std::reference_wrapper<nodes::UniformBuffer>> get_uniform_output(const std::string& name);

        GraphNode& add_attribute_output(const std::string& name);
        GraphNode& add_uniform_output(const std::string& name);

        GraphNode& add_uniform_input(const std::string& name);
        GraphNode& add_attribute_input(const std::string& name);

        std::vector<std::reference_wrapper<GraphNode>> output_nodes();
        std::vector<std::reference_wrapper<GraphNode>> input_nodes();

        const std::vector<std::reference_wrapper<const GraphNode>> output_nodes() const;
        const std::vector<std::reference_wrapper<const GraphNode>> input_nodes() const;

        /** Sets an input to the graph to be a static value. To be used when a graph is not embedded in another graph and takes input values. **/
        template<typename ValueType, unsigned int Dimensions>
        void set_input_uniform(const std::string& input_name, ptr_array<ValueType, Dimensions> value)
        {
            set_input_uniform_raw(input_name, ConnectionDataType::value<ValueType, Dimensions>(), value.raw());
        }

        /** Sets an input to the graph to be a static value. To be used when a graph is not embedded in another graph and takes input values. first_element is
            a pointer to the first element of the first bundle (e.g. x in xyzxyzxyz)**/
        template<typename ValueType, unsigned int Dimensions>
        void set_input_attribute(const std::string& input_name, const ValueType* first_element, DataBuffer::size_type attribute_length)
        {
            const ConnectionDataType& type = ConnectionDataType::value<ValueType, Dimensions>();
            set_input_attribute_raw(input_name, type, reinterpret_cast<unsigned char*>(first_element), attribute_length);
        }

        void set_input_uniform_raw(const std::string& input_name, const ConnectionDataType& data_type, const unsigned char* data_ptr);

        void set_input_attribute_raw(const std::string& input_name, const ConnectionDataType& data_type, const unsigned char* data_ptr, DataBuffer::size_type attribute_length);

        boost::optional<std::reference_wrapper<const DataBuffer>> get_input_buffer(const std::string& input_name) const;

        GraphOutputs execute() const;

        void refresh_all_sockets();

    private:
        void remove_connection(const Connection& connection);

        template<typename TBuffer>
        boost::optional<std::reference_wrapper<TBuffer>> get_buffer(const std::string& name, std::vector<std::unique_ptr<GraphNode>>& buffer_list)
        {
            auto it = std::find_if(buffer_list.begin(), buffer_list.end(), [name](std::unique_ptr<GraphNode>& node) { return node->name() == name; });
            if(it == buffer_list.end())
                return boost::none;
            return std::ref(dynamic_cast<TBuffer&>(**it));
        }

        int id_counter_;

        std::vector<std::unique_ptr<GraphNode>> nodes_;
        std::vector<std::unique_ptr<Connection>> connections_;

        std::vector<std::unique_ptr<GraphNode>> input_nodes_;
        std::vector<std::unique_ptr<GraphNode>> output_nodes_;

        PropertyCollection properties_;

        std::unordered_map<std::string, std::unique_ptr<DataBuffer>> manual_input_buffers_;
    };
}

#endif // GRAPH_H
