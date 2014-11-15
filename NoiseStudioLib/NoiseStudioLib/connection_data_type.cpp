#include "connection_data_type.h"

#include <string>

namespace noises
{
    const ConnectionDataType ConnectionDataType::undefined_(0, nullptr, 0, 0, "undefined");
    const ConnectionDataType ConnectionDataType::any_(0, nullptr, 0, 0, "any");

    template<typename ValueType>
    const char* const ConnectionDataType::ConnectionDataTypeName<ValueType>::name = "Unknown";

    const char* const ConnectionDataType::ConnectionDataTypeName<int>::name = "int";
    const char* const ConnectionDataType::ConnectionDataTypeName<float>::name = "float";
    const char* const ConnectionDataType::ConnectionDataTypeName<double>::name = "double";
    const char* const ConnectionDataType::ConnectionDataTypeName<unsigned char>::name = "unsigned char";
    const char* const ConnectionDataType::ConnectionDataTypeName<std::string>::name = "string";
}
