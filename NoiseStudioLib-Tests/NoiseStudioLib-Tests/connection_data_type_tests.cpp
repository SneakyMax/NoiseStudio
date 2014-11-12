#include "catch.h"
#include <connection_data_type.h>

using namespace noises;

TEST_CASE("Connection data types work as intended", "")
{
    SECTION("Equal data types are equal.")
    {
        REQUIRE(ConnectionDataType::equals(ConnectionDataType::value<int, 2>(), ConnectionDataType::value<int, 2>()));
    }

    SECTION("Equal data type are equal (==)")
    {
        bool is_equal = ConnectionDataType::value<int, 2>() == ConnectionDataType::value<int, 2>();
        REQUIRE(is_equal);
    }

    SECTION("Non-equal data types are not equal.")
    {
        REQUIRE_FALSE(ConnectionDataType::equals(ConnectionDataType::value<int, 2>(), ConnectionDataType::value<int, 3>()));
        REQUIRE_FALSE(ConnectionDataType::equals(ConnectionDataType::value<float, 2>(), ConnectionDataType::value<int, 2>()));
    }

    SECTION("Non-equal data types are not equal (==).")
    {
        bool is_different = ConnectionDataType::value<int, 2>() != ConnectionDataType::value<int, 3>();
        REQUIRE(is_different);

        is_different = ConnectionDataType::value<float, 2>() != ConnectionDataType::value<int, 2>();
        REQUIRE(is_different);
    }

    SECTION("Non-dimensional are equal")
    {
        bool is_same = ConnectionDataType::value<int, 2>().is<int>();
        REQUIRE(is_same);
    }

    SECTION("Non-dimensional are not equal")
    {
        bool is_same = ConnectionDataType::value<int, 2>().is<float>();
        REQUIRE_FALSE(is_same);
    }

    SECTION("1-dimensional size is correct")
    {
        auto size = ConnectionDataType::value<int, 1>().size();
        REQUIRE(size == sizeof(int));
    }

    SECTION("2-dimensional size is correct")
    {
        auto size = ConnectionDataType::value<int, 2>().size();
        REQUIRE(size == sizeof(int));
    }

    SECTION("2-dimensional full size i s correct")
    {
        auto size = ConnectionDataType::value<int, 2>().size_full();
        REQUIRE(size == sizeof(int) * 2);
    }
}
