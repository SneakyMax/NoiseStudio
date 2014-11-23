#include "connection_data_type.h"

#include <string>

namespace noises
{
    const ConnectionDataType ConnectionDataType::undefined_(0, nullptr, 0, 0, "undefined");
    const ConnectionDataType ConnectionDataType::any_(0, nullptr, 0, 0, "any");

    template<typename ValueType>
    const char* const ConnectionDataType::ConnectionDataTypeName<ValueType>::name = "Unknown";

    #define MAKE_TYPE_NAME_VALUE(TYPE, NAME) const char* const ConnectionDataType::ConnectionDataTypeName<TYPE>::name = NAME

    MAKE_TYPE_NAME_VALUE(int, "int");
    MAKE_TYPE_NAME_VALUE(unsigned int, "unsigned int");
    MAKE_TYPE_NAME_VALUE(char, "char");
    MAKE_TYPE_NAME_VALUE(unsigned char, "unsigned char");
    MAKE_TYPE_NAME_VALUE(float, "float");
    MAKE_TYPE_NAME_VALUE(double, "double");
    MAKE_TYPE_NAME_VALUE(long double, "long double");
    MAKE_TYPE_NAME_VALUE(std::string, "string");
    MAKE_TYPE_NAME_VALUE(long, "long");
    MAKE_TYPE_NAME_VALUE(unsigned long, "unsigned long");
}
