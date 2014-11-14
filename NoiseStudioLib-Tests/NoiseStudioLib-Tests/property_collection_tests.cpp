#include "catch.h"

#include <property_collection.h>

using namespace noises;

TEST_CASE("Can instantiate property collection", "")
{
    PropertyCollection c;

    REQUIRE(c.count() == 0);
}

TEST_CASE("Can add property to property collection", "")
{
    PropertyCollection c;

    c.add<int, 1>("int");

    REQUIRE(c.count() == 1);
}

TEST_CASE("Can add and retrieve property from property collection", "")
{
    PropertyCollection c;

    c.add<int, 1>("int");

    auto prop = c.get_property_by_name("int");

    SECTION("Is true optional")
    {
        REQUIRE(static_cast<bool>(prop));
    }

    SECTION("Is correct property")
    {
        REQUIRE(prop->get().name() == "int");
    }
}

TEST_CASE("Can add multiple properties of different types from property collection", "")
{
    PropertyCollection c;

    c.add<int, 1>("int");
    c.add<std::string, 1>("string");
    c.add<float, 3>("float3");

    auto int_prop = c.get_property_by_name("int");
    auto string_prop = c.get_property_by_name("string");
    auto float_prop = c.get_property_by_name("float3");

    SECTION("Are true optionals")
    {
        REQUIRE(static_cast<bool>(int_prop));
        REQUIRE(static_cast<bool>(string_prop));
        REQUIRE(static_cast<bool>(float_prop));
    }

    SECTION("Are correct properties")
    {
        REQUIRE(int_prop->get().name() == "int");
        REQUIRE(string_prop->get().name() == "string");
        REQUIRE(float_prop->get().name() == "float3");
    }
}
