#include <boost/ut.hpp>
#include <rebind/enum.hpp>

using namespace boost::ut;
using namespace boost::ut::bdd;

enum class some_enum
{
    a,
    b,
    c,
};

namespace ns
{
    enum class another_enum : std::uint8_t
    {
        x,
        y,
        z,
    };
}

// NOLINTNEXTLINE
suite<"enum"> enum_test = []()
{
    given("Non namespaced enum") = []()
    {
        expect(rebind::type_name<some_enum> == "some_enum");
        expect(rebind::nttp_name<some_enum::a> == "some_enum::a");

        expect(rebind::enum_name<some_enum::a> == "a");
        expect(rebind::enum_name<some_enum::b> == "b");

        constexpr auto fields = rebind::enum_fields<some_enum>;

        expect(fields.size() == 3);

        expect(fields.at(0).name == "a");
        expect(fields.at(0).value == some_enum::a);

        expect(fields.at(1).name == "b");
        expect(fields.at(1).value == some_enum::b);

        expect(fields.at(2).name == "c");
        expect(fields.at(2).value == some_enum::c);

        const auto name  = rebind::find_enum_name(some_enum::c);
        const auto value = rebind::find_enum_value<some_enum>("c");

        expect(name.has_value());
        expect(name == "c");

        expect(value.has_value());
        expect(value == some_enum::c);
    };

    given("Namespaced enum") = []()
    {
        expect(rebind::enum_name<ns::another_enum::x> == "x");
        expect(rebind::enum_name<ns::another_enum::y> == "y");

        constexpr auto fields = rebind::enum_fields<ns::another_enum>;

        expect(fields.size() == 3);

        expect(fields.at(0).name == "x");
        expect(fields.at(0).value == ns::another_enum::x);

        expect(fields.at(1).name == "y");
        expect(fields.at(1).value == ns::another_enum::y);

        expect(fields.at(2).name == "z");
        expect(fields.at(2).value == ns::another_enum::z);

        const auto name  = rebind::find_enum_name(ns::another_enum::z);
        const auto value = rebind::find_enum_value<ns::another_enum>("z");

        expect(name.has_value());
        expect(name == "z");

        expect(value.has_value());
        expect(value == ns::another_enum::z);
    };
};
