#ifndef CONNECTION_TYPE_H
#define CONNECTION_TYPE_H

#include <cstddef>
#include <string>
#include <boost/lexical_cast.hpp>

namespace noises
{
    class ConnectionDataType
    {
    public:
        template<typename ValueType, unsigned int Dimensions>
        constexpr static inline const ConnectionDataType& value()
        {
            return ConnectionDataValue<ValueType, Dimensions>::value();
        }

        constexpr static inline bool equals(const ConnectionDataType& a, const ConnectionDataType& b)
        {
            return &a == &b;
        }

        template<typename ValueType>
        constexpr inline bool is() const
        {
            return equals(*dimensionless_, ConnectionDataValue<ValueType, 0>::value());
        }

        inline std::size_t size() const
        {
            return size_;
        }

        inline std::size_t size_full() const
        {
            return size_full_;
        }

        const inline std::string name() const
        {
            return name_;
        }

        std::string name_description() const
        {
            return "[" + name_ + ", " + boost::lexical_cast<std::string>(dimensions_) + "]";
        }

        template<typename ValueType, unsigned int Dimensions>
        class ConnectionDataValue
        {
        public:
            constexpr static inline const ConnectionDataType& value()
            {
                return internal_value;
            }
        private:
            static const ConnectionDataType internal_value;
            static const ConnectionDataType& dimensionless_value;

            enum { size = sizeof(ValueType), size_full = sizeof(ValueType) * Dimensions };
        };

        ConnectionDataType(int dimensions, const ConnectionDataType* dimensionless, std::size_t size, std::size_t size_full, const std::string& name) :
            dimensions_(dimensions), dimensionless_(dimensionless), size_(size), size_full_(size_full), name_(name) { }

        ConnectionDataType(const ConnectionDataType&) = delete;
        ConnectionDataType(ConnectionDataType&&) = delete;
        ConnectionDataType& operator=(const ConnectionDataType&) = delete;

        inline static const ConnectionDataType& undefined() { return undefined_; }
        inline static const ConnectionDataType& any() { return any_; }

        template<typename ValueType>
        class ConnectionDataTypeName
        {
        public:
            static const char* const name;
        };

    private:
        const int dimensions_;
        const ConnectionDataType* dimensionless_;
        const std::size_t size_;
        const std::size_t size_full_;
        const std::string name_;

        static const ConnectionDataType undefined_;
        static const ConnectionDataType any_;
    };

    template<typename ValueType, unsigned int Dimensions>
    const ConnectionDataType ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::internal_value(Dimensions,
          &ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::dimensionless_value,
          ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::size,
          ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::size_full,
          ConnectionDataType::ConnectionDataTypeName<ValueType>::name);

    template<typename ValueType, unsigned int Dimensions>
    const ConnectionDataType& ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::dimensionless_value(ConnectionDataType::ConnectionDataValue<ValueType, 0>::value());

    constexpr inline bool operator==(const ConnectionDataType& l, const ConnectionDataType& r)
    {
        return ConnectionDataType::equals(l, r);
    }

    constexpr inline bool operator!=(const ConnectionDataType& l, const ConnectionDataType& r)
    {
        return !(l == r);
    }

    template<> class ConnectionDataType::ConnectionDataTypeName<int> { public: static const char* const name; };
    template<> class ConnectionDataType::ConnectionDataTypeName<float> { public: static const char* const name; };
    template<> class ConnectionDataType::ConnectionDataTypeName<double> { public: static const char* const name; };
    template<> class ConnectionDataType::ConnectionDataTypeName<unsigned char> { public: static const char* const name; };
    template<> class ConnectionDataType::ConnectionDataTypeName<std::string> { public: static const char* const name; };
}

#endif // CONNECTION_TYPE_H
