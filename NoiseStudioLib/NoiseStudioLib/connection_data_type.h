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
        constexpr static const ConnectionDataType& value()
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

        int dimensions() const
        {
            return dimensions_;
        }

        std::string name_description() const
        {
            return "[" + name_ + ", " + boost::lexical_cast<std::string>(dimensions_) + "]";
        }

        bool is_same_type(const ConnectionDataType& other) const
        {
            return equals(*dimensionless_, *other.dimensionless_);
        }

        template<typename ValueType>
        static const ConnectionDataType& dynamic(int dimensions)
        {
            switch(dimensions)
            {
                case 0:
                    return ConnectionDataType::value<ValueType, 0>();
                case 1:
                    return ConnectionDataType::value<ValueType, 1>();
                case 2:
                    return ConnectionDataType::value<ValueType, 2>();
                case 3:
                    return ConnectionDataType::value<ValueType, 3>();
                case 4:
                    return ConnectionDataType::value<ValueType, 4>();
                case 5:
                    return ConnectionDataType::value<ValueType, 5>();
                case 6:
                    return ConnectionDataType::value<ValueType, 6>();
                case 7:
                    return ConnectionDataType::value<ValueType, 7>();
                case 8:
                    return ConnectionDataType::value<ValueType, 8>();
                case 9:
                    return ConnectionDataType::value<ValueType, 9>();
                case 10:
                    return ConnectionDataType::value<ValueType, 10>();
            }

            throw std::invalid_argument("Unsupported number of dimensions. Only supports 1-10 dimensions.");
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

    #define MAKE_TYPE_NAME(type) template<> class ConnectionDataType::ConnectionDataTypeName<type> { public: static const char* const name; }

    MAKE_TYPE_NAME(int);
    MAKE_TYPE_NAME(unsigned int);
    MAKE_TYPE_NAME(float);
    MAKE_TYPE_NAME(double);
    MAKE_TYPE_NAME(long double);
    MAKE_TYPE_NAME(char);
    MAKE_TYPE_NAME(unsigned char);
    MAKE_TYPE_NAME(std::string);
    MAKE_TYPE_NAME(long);
    MAKE_TYPE_NAME(unsigned long);


}

#endif // CONNECTION_TYPE_H
