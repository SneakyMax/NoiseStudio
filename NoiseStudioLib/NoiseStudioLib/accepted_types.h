#ifndef INPUT_OUTPUT_ACCEPTED_TYPES_H
#define INPUT_OUTPUT_ACCEPTED_TYPES_H

#include <vector>
#include <functional>

namespace noises
{
    class ConnectionDataType;

    class AcceptedTypes
    {
    public:
        void add(const ConnectionDataType& data_type);
        void remove(const ConnectionDataType& data_type);
        bool supports(const ConnectionDataType& data_type) const;
    private:
        std::vector<std::reference_wrapper<const ConnectionDataType>> supported_types_;
    };
}

#endif // INPUT_OUTPUT_ACCEPTED_TYPES_H
