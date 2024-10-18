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

        constexpr auto values = rebind::enum_values<some_enum>;

        expect(values.size() == 3);

        expect(values[0] == some_enum::a);
        expect(rebind::enum_name<values[0]> == "a");

        expect(values[1] == some_enum::b);
        expect(rebind::enum_name<values[1]> == "b");

        expect(values[2] == some_enum::c);
        expect(rebind::enum_name<values[2]> == "c");
    };

    given("Namespaced enum") = []()
    {
        expect(rebind::type_name<ns::another_enum>.ends_with("another_enum"));
        expect(rebind::nttp_name<ns::another_enum::x>.ends_with("another_enum::x"));

        expect(rebind::enum_name<ns::another_enum::x> == "x");
        expect(rebind::enum_name<ns::another_enum::y> == "y");

        constexpr auto values = rebind::enum_values<ns::another_enum>;

        expect(values.size() == 3);

        expect(values[0] == ns::another_enum::x);
        expect(rebind::enum_name<values[0]> == "x");

        expect(values[1] == ns::another_enum::y);
        expect(rebind::enum_name<values[1]> == "y");

        expect(values[2] == ns::another_enum::z);
        expect(rebind::enum_name<values[2]> == "z");
    };
};
