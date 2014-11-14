#include "accepted_types.h"
#include "connection_data_type.h"
#include "utils.h"

namespace noises
{
    void AcceptedTypes::add(const ConnectionDataType& data_type)
    {
        if(supports(data_type))
            return;
        supported_types_.push_back(std::reference_wrapper<const ConnectionDataType>(data_type));
    }

    void AcceptedTypes::remove(const ConnectionDataType& data_type)
    {
        utils::remove_by_pointer(supported_types_, &data_type);
    }

    bool AcceptedTypes::supports(const ConnectionDataType& data_type) const
    {
        for(auto supported_type : supported_types_)
        {
            if(ConnectionDataType::equals(supported_type, data_type))
                return true;
        }
        return false;
    }
}
