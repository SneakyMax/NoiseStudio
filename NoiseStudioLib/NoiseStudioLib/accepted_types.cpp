#include "accepted_types.h"
#include "connection_data_type.h"

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
        std::vector<std::reference_wrapper<const ConnectionDataType>>::iterator it;
        for(it = supported_types_.begin(); it != supported_types_.end(); ++it)
        {
            if(ConnectionDataType::equals(*it, data_type))
                break;
        }

        if(it != supported_types_.end())
        {
            supported_types_.erase(it);
        }
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
