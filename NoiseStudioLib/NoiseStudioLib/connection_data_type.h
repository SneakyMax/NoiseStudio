#ifndef CONNECTION_TYPE_H
#define CONNECTION_TYPE_H

#include <cstddef>

namespace noises
{
    class ConnectionDataType
    {
    public:
        template<typename ValueType, unsigned int Dimensions>
        static inline const ConnectionDataType& value()
        {
            return ConnectionDataValue<ValueType, Dimensions>::value();
        }

        static inline bool equals(const ConnectionDataType& a, const ConnectionDataType& b)
        {
            return &a == &b;
        }

        template<typename ValueType>
        inline bool is() const
        {
            return equals(dimensionless_, ConnectionDataValue<ValueType, 0>::value());
        }

        inline std::size_t size() const
        {
            return size_;
        }

        inline std::size_t size_full() const
        {
            return size_full_;
        }

        template<typename ValueType, unsigned int Dimensions>
        class ConnectionDataValue
        {
        public:
            static inline const ConnectionDataType& value()
            {
                return internal_value;
            }
        private:
            static const ConnectionDataType internal_value;
            static const ConnectionDataType& dimensionless_value;

            enum { size = sizeof(ValueType), size_full = sizeof(ValueType) * Dimensions };
        };

        ConnectionDataType(int dimensions, const ConnectionDataType& dimensionless, std::size_t size, std::size_t size_full) :
            dimensions_(dimensions), dimensionless_(dimensionless), size_(size), size_full_(size_full) { }

        ConnectionDataType(const ConnectionDataType&) = delete;
        ConnectionDataType(ConnectionDataType&&) = delete;
        ConnectionDataType& operator=(const ConnectionDataType&) = delete;

    private:
        int dimensions_;
        const ConnectionDataType& dimensionless_;
        std::size_t size_;
        std::size_t size_full_;
    };

    template<typename ValueType, unsigned int Dimensions>
    const ConnectionDataType ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::internal_value(Dimensions,
          ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::dimensionless_value,
          ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::size,
          ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::size_full);

    template<typename ValueType, unsigned int Dimensions>
    const ConnectionDataType& ConnectionDataType::ConnectionDataValue<ValueType, Dimensions>::dimensionless_value(ConnectionDataType::ConnectionDataValue<ValueType, 0>::value());

    inline bool operator==(const ConnectionDataType& l, const ConnectionDataType& r)
    {
        return ConnectionDataType::equals(l, r);
    }

    inline bool operator!=(const ConnectionDataType& l, const ConnectionDataType& r)
    {
        return !(l == r);
    }
}

#endif // CONNECTION_TYPE_H
