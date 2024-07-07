#include <boost/ut.hpp>
#include <rebind/enum.hpp>

using namespace boost::ut;

enum class some_enum
{
    a,
    b,
    c,
};

// NOLINTNEXTLINE
suite<"enum"> enum_test = []()
{
    expect(rebind::type_name<some_enum>.rfind("some_enum") != std::string_view::npos);
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
