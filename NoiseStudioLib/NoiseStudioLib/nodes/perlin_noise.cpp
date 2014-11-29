#include "perlin_noise.h"

#include <array>
#include <boost/random.hpp>
#include <vector>

#include "composite_data_buffer.h"

namespace noises {
namespace nodes
{
    struct PerlinNoiseData
    {
        PerlinNoiseData() { }

        std::unique_ptr<boost::random::mt19937> rng;

        std::vector<signed char> vectors;

        int dimensions;
    };

    PerlinNoise::PerlinNoise()
    {
        seed_socket_ = &inputs().add("Seed", SocketType::uniform);
        seed_socket_->set_accepts(ConnectionDataType::value<long, 1>());

        // Just a vector2 to start out with
        points_socket_ = &inputs().add("Points", SocketType::attribute);
        points_socket_->set_accepts(ConnectionDataType::value<float, 2>());
        points_socket_->set_accepts(ConnectionDataType::value<float, 3>());
        points_socket_->set_accepts(ConnectionDataType::value<float, 4>());
        points_socket_->set_accepts(ConnectionDataType::value<float, 5>());
        points_socket_->set_accepts(ConnectionDataType::value<float, 6>());

        output_socket_ = &outputs().add("Output", ConnectionDataType::value<float, 1>(), SocketType::attribute);
    }

    std::string PerlinNoise::node_name() const
    {
        return "Perlin Noise";
    }

    void PerlinNoise::execute_uniforms(const CompositeDataBuffer &input, DataBuffer &output) const
    {
        long seed = input.get_uniform<long, 1>(*seed_socket_);

        std::shared_ptr<PerlinNoiseData> data(new PerlinNoiseData);

        data->rng.reset(new boost::random::mt19937);
        data->rng->seed(seed);

        int dimensions = points_socket_->connection()->get().data_type().dimensions();
        load_hypercube_edge_vectors(*data, dimensions);

        utils::shuffle_group(data->vectors, *data->rng, dimensions);

        output.set_scratch(0, std::move(data));
    }

    void PerlinNoise::load_hypercube_edge_vectors(PerlinNoiseData &data, int dimensions) const
    {
        switch(dimensions)
        {
            case 0:
                throw std::logic_error("Cannot have 0 dimension noise.");
            case 1:
                throw std::logic_error("Cannot have 1 dimension perlin noise.");
            case 2:
                data.vectors = HypercubeEdges<2>::edge_vectors_flattened();
                break;
            case 3:
                data.vectors = HypercubeEdges<3>::edge_vectors_flattened();
                break;
            case 4:
                data.vectors = HypercubeEdges<4>::edge_vectors_flattened();
                break;
            case 5:
                data.vectors = HypercubeEdges<5>::edge_vectors_flattened();
                break;
            case 6:
                data.vectors = HypercubeEdges<6>::edge_vectors_flattened();
        }

        data.dimensions = dimensions;
    }

    void PerlinNoise::execute_attributes(const CompositeDataBuffer &input, DataBuffer &output, DataBuffer::size_type index) const
    {
        PerlinNoiseData& data = *output.get_scratch_ref<std::shared_ptr<PerlinNoiseData>>(0);
        int dimensions = data.dimensions;

        const float* attribute_point = get_input_point_attribute(index, input, dimensions);

        std::vector<int> dimension_starts(dimensions);
        std::vector<std::vector<int>> points;
        std::vector<int> gradient_indexes;

        int num_gradients = data.vectors.size() / dimensions;

        // Get floor(x), floor(y), floor(z) etc
        for(int i = 0; i < dimensions; i++)
        {
            dimension_starts[i] = std::floor(attribute_point[i]);
        }

        // Get all points in the hypercube surrounding the input point
        get_point_permutations(dimension_starts, dimensions, std::vector<int>(), points);

        // Get all rng gradients for each point. Hash x/y/z to get index into gradients array
        for(unsigned int i = 0; i < points.size(); i++)
        {
            std::vector<int>& point = points[i];
            unsigned int sum = 0;
            for(int j = 0; j < dimensions; j++)
            {
                static const unsigned int primes[] { 1699, 2237, 2671, 3571, 1949, 2221, 3469, 3083 };
                sum += point[j] * point[j] * primes[j]; //Hopefully this has enough entropy and random enough results
            }

            gradient_indexes.push_back(sum % num_gradients);
        }

        // Because of how the get_point_permutations algorithm works, adjacent verticies are in pairs recursively
        // There will always be an even number of points (2^n)
        std::vector<float> out(points.size());

        // vector from the first hypercube vertex to the input point (main diagonal) i.e. uv
        std::vector<float> uvs(dimensions);

        for(int i = 0; i < dimensions; i++)
        {
            uvs[i] = attribute_point[i] - points[0][i];
        }

        // Number of vertexes for an n-dimensional hypercube is 2^n
        int num_points = std::pow(2, dimensions);

        // Find dot product for every vertex.
        // Multiply the vector from the hypercube vertex to the input point, and the gradient at the hypercube vertex
        for(int i = 0; i < num_points; i++)
        {
            std::vector<int>& point = points[i];
            int gradient_index = gradient_indexes[i] * dimensions;

            // In theory, the dot products will be -1 to 1
            float dot = 0;

            for(int j = 0; j < dimensions; j++)
            {
                // gradient value (for this dimension) at hypercube vertex
                signed char gradient_u = data.vectors[gradient_index + j];

                // input point (in this dimension)
                float attr_p = attribute_point[j];

                // vector (for this dimension) from hypercube vertex to input point
                float u = attr_p - point[j];

                dot += u * gradient_u;
            }

            out[i] = dot;
        }

        // Collapse the dimensions down
        int current_dimension = dimensions;
        for(; num_points > 1; num_points /= 2)
        {
            // Take the dot products two at a time and interpolate between then
            int half_num = num_points / 2;
            for(int i = 0; i < half_num; i++)
            {
                int index = i * 2;

                float x_1 = out[index];
                float x_2 = out[index + 1];

                float u = uvs[current_dimension - 1];
                float interpolated = 6 * std::pow(u, 5) - (15 * std::pow(u, 4)) + (10 * std::pow(u, 3));

                out[i] = (x_1 * (1.0f - interpolated)) + (x_2 * interpolated);
            }

            current_dimension--;
        }

        output.set_attribute<float, 1>(*output_socket_, index, &out[0]);
    }

    const float* PerlinNoise::get_input_point_attribute(DataBuffer::size_type index, const CompositeDataBuffer& input, int dimensions) const
    {
        switch(dimensions)
        {
            case 2:
                return &input.get_attribute<float, 2>(*points_socket_, index);
            case 3:
                return &input.get_attribute<float, 3>(*points_socket_, index);
            case 4:
                return &input.get_attribute<float, 4>(*points_socket_, index);
            case 5:
                return &input.get_attribute<float, 5>(*points_socket_, index);
            case 6:
                return &input.get_attribute<float, 7>(*points_socket_, index);
        }
        throw std::logic_error("Not supported");
    }

    void PerlinNoise::get_point_permutations(std::vector<int>& dimension_starts, int dimensions, std::vector<int> point, std::vector<std::vector<int>>& points)
    {
        int dimension = point.size();
        if(dimension == dimensions)
        {
            points.push_back(point);
        }
        else
        {
            int start = dimension_starts[dimension];

            point.push_back(start);
            get_point_permutations(dimension_starts, dimensions, point, points);

            point[dimension] = start + 1;
            get_point_permutations(dimension_starts, dimensions, point, points);
        }
    }
} }
