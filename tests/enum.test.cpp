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
    enum class another_enum
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

        constexpr auto values = rebind::enum_values<some_enum>;

        expect(values.size() == 3);

        expect(values.at(0).name == "a");
        expect(values.at(0).value == some_enum::a);

        expect(values.at(1).name == "b");
        expect(values.at(1).value == some_enum::b);

        expect(values.at(2).name == "c");
        expect(values.at(2).value == some_enum::c);

        const auto value = rebind::enum_value(some_enum::c);
        expect(value.has_value());

        expect(value->name == "c");
        expect(value->value == some_enum::c);
    };

    given("Namespaced enum") = []()
    {
        expect(rebind::enum_name<ns::another_enum::x> == "x");
        expect(rebind::enum_name<ns::another_enum::y> == "y");

        constexpr auto values = rebind::enum_values<ns::another_enum>;

        expect(values.size() == 3);

        expect(values.at(0).name == "x");
        expect(values.at(0).value == ns::another_enum::x);

        expect(values.at(1).name == "y");
        expect(values.at(1).value == ns::another_enum::y);

        expect(values.at(2).name == "z");
        expect(values.at(2).value == ns::another_enum::z);

        const auto value = rebind::enum_value(ns::another_enum::z);
        expect(value.has_value());

        expect(value->name == "z");
        expect(value->value == ns::another_enum::z);
    };
};
