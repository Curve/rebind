#include <boost/ut.hpp>

#include <rebind/enum.hpp>
#include <rebind/utils/enum.hpp>

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
suite<"enum"> enum_test = []
{
    given("Non namespaced enum") = []
    {
        expect(rebind::type_name<some_enum> == "some_enum");
        expect(rebind::nttp_name<some_enum::a> == "some_enum::a");

        expect(rebind::enum_name<some_enum::a> == "a");
        expect(rebind::enum_name<some_enum::b> == "b");

        expect(rebind::utils::find_enum_name<some_enum>(some_enum::a) == "a");
        expect(rebind::utils::find_enum_value<some_enum>("b") == some_enum::b);

        static constexpr auto values = rebind::enum_values<some_enum>;
        static constexpr auto names  = rebind::utils::enum_names<some_enum>;

        expect(values.size() == 3);
        expect(names.size() == 3);

        expect(rebind::enum_name<values[0]> == "a");
        expect(values[0] == some_enum::a);
        expect(names[0] == "a");

        expect(rebind::enum_name<values[1]> == "b");
        expect(values[1] == some_enum::b);
        expect(names[1] == "b");

        expect(rebind::enum_name<values[2]> == "c");
        expect(values[2] == some_enum::c);
        expect(names[2] == "c");
    };

    given("Namespaced enum") = []
    {
        expect(rebind::type_name<ns::another_enum>.ends_with("another_enum"));
        expect(rebind::nttp_name<ns::another_enum::x>.ends_with("another_enum::x"));

        expect(rebind::enum_name<ns::another_enum::x> == "x");
        expect(rebind::enum_name<ns::another_enum::y> == "y");

        static constexpr auto values = rebind::enum_values<ns::another_enum>;
        static constexpr auto names  = rebind::utils::enum_names<ns::another_enum>;

        expect(values.size() == 3);
        expect(names.size() == 3);

        expect(rebind::enum_name<values[0]> == "x");
        expect(values[0] == ns::another_enum::x);
        expect(names[0] == "x");

        expect(values[1] == ns::another_enum::y);
        expect(rebind::enum_name<values[1]> == "y");
        expect(names[1] == "y");

        expect(rebind::enum_name<values[2]> == "z");
        expect(values[2] == ns::another_enum::z);
        expect(names[2] == "z");
    };
};
