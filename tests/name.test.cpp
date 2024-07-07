#include <boost/ut.hpp>
#include <rebind/name.hpp>

using namespace boost::ut;

// NOLINTNEXTLINE
suite<"name"> name_test = []()
{
    expect(rebind::type_name<int> == "int");
    expect(rebind::type_name<float> == "float");
    expect(rebind::type_name<double> == "double");

    expect(rebind::nttp_name<true> == "true");
    expect(rebind::nttp_name<false> == "false");
};
