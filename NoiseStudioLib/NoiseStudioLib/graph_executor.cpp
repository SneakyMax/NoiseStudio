#include "graph_executor.h"

#include <boost/format.hpp>

#include "graph.h"
#include "graph_node.h"
#include "graph_validator.h"
#include "composite_data_buffer.h"

namespace noises
{
    GraphExecutor::GraphExecutor(const Graph &graph) : graph_(graph)
    {

    }

    const Graph& GraphExecutor::graph()
    {
        return graph_;
    }

    GraphOutputs GraphExecutor::execute()
    {
        auto validation = validate_graph();
        if(!validation)
            throw std::logic_error("Could not execute graph. It is invalid.");

        get_topological_order();
        buffer_stack_.resize(topological_order_.size());

        return execute_internal();
    }

    GraphOutputs GraphExecutor::execute_internal()
    {
        while(buffer_stack_.size() > 1)
        {
            std::vector<int> current_nodes_to_calculate = topological_order_.back();

            for(int node_id : current_nodes_to_calculate)
            {
                execute_node(node_id);
            }

            // Release any memory we don't need any more
            topological_order_.pop_back();
            buffer_stack_.pop_back();
        }

        GraphOutputs outputs;

        for(const GraphNode& output : graph_.output_nodes())
        {
            // Attribute length is irrelevant here
            outputs.add(output.name(), extract_buffer(get_buffer(output.id(), 0)));
        }

        return outputs;
    }

    void GraphExecutor::execute_node(int node_id)
    {
        const GraphNode& node = *graph_.get_node_by_id(node_id);
        auto buffers = get_node_dependency_buffers(node);
        std::size_t attribute_length = get_attribute_length(node);

        DataBuffer& output_buffer = get_buffer(node.id(), attribute_length);
        CompositeDataBuffer input_buffer(attribute_length);
        std::vector<unsigned char> empty_buffer;

        add_attribute_dependencies(input_buffer, node, buffers, empty_buffer);
        add_uniform_dependencies(input_buffer, node, buffers);

        output_buffer.add(node.outputs());

        node.execute_uniforms(input_buffer, output_buffer);

        for(DataBuffer::size_type i = 0; i < attribute_length; i++)
        {
            node.execute_attributes(input_buffer, output_buffer, i);
        }
    }

    void GraphExecutor::add_attribute_dependencies(CompositeDataBuffer &input_buffer, const GraphNode &node,
                                                   std::unordered_map<int, std::reference_wrapper<DataBuffer>>& buffers, std::vector<unsigned char>& empty_buffer)
    {
        for(const InputSocket& attribute_socket : node.inputs().attribute_sockets())
        {
            auto possible_connection = attribute_socket.connection();
            if(possible_connection)
            {
                const Connection& connection = *possible_connection;
                const OutputSocket& output_socket = connection.output();
                const GraphNode* dependency_node = output_socket.parent();
                int dependency_id = dependency_node->id();

                DataBuffer& dependency_buffer = buffers.at(dependency_id);
                input_buffer.add_attribute(connection.data_type(), dependency_buffer.get_memory_block(output_socket.index()));
                continue;
            }

            // If it's graph internal node and doesn't have an input connection, assume it's an input to the graph manually
            if(!(node.is_graph_internal_node() && attribute_socket.optional()))
            {
                input_buffer.add_attribute(ConnectionDataType::undefined(), empty_buffer);
                continue;
            }

            auto graph_input = graph_.get_input_buffer(node.name());
            if(graph_input)
            {
                const DataBuffer& input_attribute_buffer = graph_input->get();
                input_buffer.add_attribute(input_attribute_buffer.get_attribute_type(0), input_attribute_buffer.get_memory_block(0));
            }
            else
            {
                input_buffer.add_attribute(ConnectionDataType::undefined(), empty_buffer);
            }
        }

        assert(input_buffer.num_attributes() == node.inputs().attribute_sockets().size());
    }

    void GraphExecutor::add_uniform_dependencies(CompositeDataBuffer &input_buffer, const GraphNode &node,
                                                 std::unordered_map<int, std::reference_wrapper<DataBuffer>>& buffers)
    {
        for(const InputSocket& uniform_socket : node.inputs().uniform_sockets())
        {
            auto possible_connection = uniform_socket.connection();
            if(possible_connection)
            {
                const Connection& connection = *possible_connection;
                const OutputSocket& output_socket = connection.output();
                const GraphNode* dependency_node = output_socket.parent();
                int dependency_id = dependency_node->id();

                DataBuffer& dependency_buffer = buffers.at(dependency_id);
                input_buffer.add_uniform(connection.data_type(), dependency_buffer.get_uniform_raw(output_socket, connection.data_type()));
                continue;
            }

            if(!(node.is_graph_internal_node() && uniform_socket.optional()))
            {
                input_buffer.add_uniform(ConnectionDataType::undefined(), nullptr);
                continue;
            }

            auto graph_input = graph_.get_input_buffer(node.name());
            if(graph_input)
            {
                const DataBuffer& input_uniform_buffer = graph_input->get();
                input_buffer.add_uniform(input_uniform_buffer.get_uniform_type(0), input_uniform_buffer.get_uniform_block(0));
            }
            else
            {
                input_buffer.add_uniform(ConnectionDataType::undefined(), nullptr);
            }
        }

        assert(input_buffer.num_uniforms() == node.inputs().uniform_sockets().size());
    }

    std::unordered_map<int, std::reference_wrapper<DataBuffer>> GraphExecutor::get_node_dependency_buffers(const GraphNode &node)
    {
        std::unordered_map<int, std::reference_wrapper<DataBuffer>> out;

        auto dependencies = get_node_dependencies(node);
        for(int dependency_id : dependencies)
        {
            const GraphNode& dependency_node = *graph_.get_node_by_id(dependency_id);
            DataBuffer& buffer = get_buffer(dependency_id, get_attribute_length(dependency_node));
            out.emplace(std::make_pair(dependency_id, std::ref(buffer)));
        }

        return out;
    }

    std::vector<int> GraphExecutor::get_node_dependencies(const GraphNode& node)
    {
        std::vector<int> out;
        for(const InputSocket& socket : node.inputs().all_sockets())
        {
            auto possible_connection = socket.connection();
            if(possible_connection)
            {
                const Connection& connection = *possible_connection;
                int dependency_id = connection.output().parent()->id();

                if(std::find(out.begin(), out.end(), dependency_id) == out.end())
                    out.push_back(dependency_id);
            }
        }
        return out;
    }

    DataBuffer& GraphExecutor::get_buffer(int node_id, std::size_t buffer_attribute_length)
    {
        // Get the stack position in topological_order_
        // Start at -1 because first iteration (for a size of 1) increments it to 0
        int i = -1;
        auto it = topological_order_.begin();
        for(; it != topological_order_.end(); ++it)
        {
            i++;
            if(std::find(it->begin(), it->end(), node_id) != it->end())
                break;
        }

        if(it == topological_order_.end())
        {
            // If it's not in the topological order any more, try just searching through the buffer stack
            for(auto& buffer_level : buffer_stack_)
            {
                for(auto& pair : buffer_level)
                {
                    if(std::get<0>(pair) == node_id)
                    {
                        return *std::get<1>(pair);
                    }
                }
            }

            throw std::logic_error("Could not find a position for node " + boost::lexical_cast<std::string>(node_id) + " in the topological_order_");
        }

        // Find the (stack level - 1) in buffer_stack_
        // Stack level -1 because we need (stack level)'s buffer in (stack level - 1)
        auto it_j = buffer_stack_.begin();
        for(int j = 1; j < i; ++j, ++it_j) { }

        if(it_j == buffer_stack_.end())
            throw std::logic_error("Could not find a position for node " + boost::lexical_cast<std::string>(node_id) + " in the buffer_stack_");

        auto& buffer_level = *it_j;

        // Fint the matching node_id in the pair in the buffer level vector
        auto it_q = buffer_level.begin();
        for(; it_q != buffer_level.end(); ++it_q)
        {
            if(std::get<0>(*it_q) == node_id)
                break;
        }

        if(it_q == buffer_level.end())
        {
            // We need to allocate a new buffer
            std::unique_ptr<DataBuffer> buffer(new DataBuffer(buffer_attribute_length));
            DataBuffer& buffer_ref = *buffer;

            auto pair = std::make_pair(node_id, std::move(buffer));
            buffer_level.push_back(std::move(pair));

            return buffer_ref;
        }
        else
        {
            auto& pair = *it_q;
            return *std::get<1>(pair);
        }
    }

    std::unique_ptr<DataBuffer> GraphExecutor::extract_buffer(const DataBuffer &buffer)
    {
        std::unique_ptr<DataBuffer> out;

        for(auto& buffer_level : buffer_stack_)
        {
            for(auto& pair : buffer_level)
            {
                std::unique_ptr<DataBuffer>& pair_ptr = std::get<1>(pair);
                if(pair_ptr.get() == &buffer)
                {
                    std::swap(out, pair_ptr);
                }
            }
        }

        if(!out)
            throw std::logic_error("DataBuffer wasn't in buffer stack??");

        return out;
    }

    std::size_t GraphExecutor::get_attribute_length(const GraphNode &node)
    {
        auto override = node.get_attribute_override();
        if(override)
        {
            return override->length();
        }

        for(const InputSocket& socket : node.inputs().attribute_sockets())
        {
            auto possible_connection = socket.connection();
            if(!possible_connection)
                continue;

            const Connection& connection = *possible_connection;
            const GraphNode* output_node = connection.output().parent();
            return get_attribute_length(*output_node);
        }

        // Undefined attribute length, assume no attributes
        return 0;
    }

    ValidationResults GraphExecutor::validate_graph() const
    {
        GraphValidator validator(graph_);
        return validator.validate();
    }

    void GraphExecutor::get_topological_order()
    {
        std::deque<std::vector<int>> out;

        // Push a dummy first because it looks to (stack level + 1) as the storage space
        out.push_back(std::vector<int>());

        std::vector<int> root;
        for(const GraphNode& output_node : graph_.output_nodes())
        {
            root.push_back(output_node.id());
        }
        out.push_back(root);

        // Won't run infinitely because the graph has been validated as non-cyclic
        while(true)
        {
            std::vector<int> next_stack_layer;

            for(int node_id : out.back())
            {
                const GraphNode& node = *graph_.get_node_by_id(node_id);

                for(const InputSocket& socket : node.inputs().all_sockets())
                {
                    auto possible_connection = socket.connection();
                    if(possible_connection)
                    {
                        const Connection& connection = *possible_connection;
                        const GraphNode* connection_output = connection.output().parent();

                        int id = connection_output->id();

                        if(std::find(next_stack_layer.begin(), next_stack_layer.end(), id) == next_stack_layer.end())
                            next_stack_layer.push_back(id);
                    }
                }
            }

            if(next_stack_layer.size() > 0)
            {
                out.push_back(next_stack_layer);
            }
            else break;
        }

        topological_order_ = out;
    }
}
