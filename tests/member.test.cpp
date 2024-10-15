#include <boost/ut.hpp>
#include <rebind/member.hpp>

using namespace boost::ut;

struct simple
{
    int x;
    float y;
    double z;

    struct
    {
        bool test;
    } inner;
};

// NOLINTNEXTLINE
suite<"member"> member_test = []()
{
    expect(rebind::arity<simple> == 4);

    expect(rebind::member_name<&simple::x> == "x");
    expect(rebind::member_name<&simple::y> == "y");
    expect(rebind::member_name<&simple::z> == "z");
    expect(rebind::member_name<&simple::inner> == "inner");

    constexpr auto members = rebind::members<simple>;
    constexpr auto names   = rebind::member_names<simple>;

    expect(names.size() == 4);
    expect(std::tuple_size_v<decltype(members)> == 4);

    expect(names.at(0) == "x");
    expect(std::get<0>(members).name == "x");

    expect(names.at(1) == "y");
    expect(std::get<1>(members).name == "y");

    expect(names.at(2) == "z");
    expect(std::get<2>(members).name == "z");

    expect(names.at(3) == "inner");
    expect(std::get<3>(members).name == "inner");

    constexpr auto field = rebind::inspect<simple, 1>;
    using field_t        = decltype(field);

    expect(field.name == "y");

    expect(eq(field_t::index, 1));
    expect(std::is_same_v<field_t::type, float>);

    simple instance{1, 2, 3, {true}};

    const auto tuple = rebind::to_tuple(instance);
    expect(std::tuple_size_v<decltype(tuple)> == 4);

    expect(std::get<0>(tuple) == 1);
    expect(std::get<1>(tuple) == 2);
    expect(std::get<2>(tuple) == 3);
    expect(std::get<3>(tuple).test);

    rebind::get<0>(instance)      = 500;
    rebind::get<3>(instance).test = false;

    expect(std::get<0>(tuple) == 500);
    expect(instance.x == 500);

    expect(!std::get<3>(tuple).test);
    expect(!instance.inner.test);
};
