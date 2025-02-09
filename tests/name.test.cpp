#include <boost/ut.hpp>
#include <rebind/name.hpp>
#include <rebind/utils/name.hpp>

using namespace boost::ut;

namespace ns
{
    struct some_struct
    {
    };

    struct some_class
    {
    };
} // namespace ns

struct some_struct
{
};

struct some_class
{
};

// NOLINTNEXTLINE
suite<"name"> name_test = []
{
    expect(rebind::type_name<int> == "int");
    expect(rebind::type_name<float> == "float");
    expect(rebind::type_name<double> == "double");

    expect(rebind::nttp_name<true> == "true");
    expect(rebind::nttp_name<false> == "false");

    expect(rebind::type_name<some_struct> == "some_struct");
    expect(rebind::type_name<ns::some_struct>.ends_with("some_struct"));

    expect(rebind::utils::pure_name<some_struct> == "some_struct");
    expect(rebind::utils::pure_name<ns::some_struct> == "some_struct");

    expect(rebind::type_name<some_class> == "some_class");
    expect(rebind::type_name<ns::some_class>.ends_with("some_class"));

    expect(rebind::utils::pure_name<some_class> == "some_class");
    expect(rebind::utils::pure_name<ns::some_class> == "some_class");
};
