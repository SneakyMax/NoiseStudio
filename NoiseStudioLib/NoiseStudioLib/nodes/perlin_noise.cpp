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

        data->rng->seed(seed);
        load_hypercube_edge_vectors(*data);

        utils::shuffle_group(data->vectors, *data->rng, 4);

        output.set_scratch(0, std::move(data));
    }

    void PerlinNoise::load_hypercube_edge_vectors(PerlinNoiseData &data) const
    {
        int dimensions = points_socket_->connection()->get().data_type().dimensions();

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
        PerlinNoiseData& data = *output.get_scratch_ref<std::unique_ptr<PerlinNoiseData>>(0);
        int dimensions = data.dimensions;

        const float* point = get_input_point_attribute(index, input, dimensions);


        std::vector<int> dimension_starts;
        std::vector<std::vector<int>> points;
        std::vector<int> gradient_indexes;

        int num_gradients = data.vectors.size();

        // Get floor(x), floor(y), floor(z) etc
        for(int i = 0; i < dimensions; i++)
        {
            dimension_starts.push_back(std::floor(point[i]));
        }

        // Get all points in the hypercube
        for(int i = 0; i < dimensions; i++)
        {
            get_point_permutations(dimension_starts, dimensions, std::vector<int>(), points);
        }

        // Get all rng gradients for each point
        for(unsigned int i = 0; i < points.size(); i++)
        {
            std::vector<int>& point = points[i];
            int sum = 0;
            for(int j = 0; j < dimensions; j++)
            {
                static const int primes[] { 1699, 2237, 2671, 3571, 1949, 2221, 3469, 3083 };
                sum += point[j] * point[j] * primes[j]; //Hopefully this has enough entropy and random enough results
            }

            gradient_indexes.push_back(sum % num_gradients);
        }
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
