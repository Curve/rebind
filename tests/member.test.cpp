#include <boost/ut.hpp>

#include <rebind/member.hpp>
#include <rebind/utils/member.hpp>

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
suite<"member"> member_test = []
{
    expect(rebind::arity<simple> == 4);

    expect(rebind::member_name<&simple::x> == "x");
    expect(rebind::member_name<&simple::y> == "y");
    expect(rebind::member_name<&simple::z> == "z");
    expect(rebind::member_name<&simple::inner> == "inner");

    static constexpr auto members = rebind::members<simple>;
    static constexpr auto names   = rebind::utils::member_names<simple>;

    expect(std::tuple_size_v<decltype(members)> == 4);
    expect(names.size() == 4);

    expect(std::get<0>(members).name == "x");
    expect(std::get<1>(members).name == "y");
    expect(std::get<2>(members).name == "z");

    expect(names[0] == "x");
    expect(names[1] == "y");
    expect(names[2] == "z");

    expect(std::get<3>(members).name == "inner");
    expect(names[3] == "inner");

    expect(std::get<0>(members).index == 0);
    expect(std::get<1>(members).index == 1);
    expect(std::get<2>(members).index == 2);

    expect(std::get<3>(members).index == 3);

    expect(std::same_as<std::tuple_element_t<0, decltype(members)>::type, int>);
    expect(std::same_as<std::tuple_element_t<1, decltype(members)>::type, float>);
    expect(std::same_as<std::tuple_element_t<2, decltype(members)>::type, double>);
    expect(std::is_class_v<std::tuple_element_t<3, decltype(members)>::type>);

    simple instance{1, 2, 3, {true}};

    const auto tuple = rebind::to_tuple(instance);
    expect(std::tuple_size_v<decltype(tuple)> == 4);

    expect(std::get<0>(tuple) == 1);
    expect(std::get<1>(tuple) == 2);
    expect(std::get<2>(tuple) == 3);
    expect(std::get<3>(tuple).test);

    std::get<0>(tuple) = 500;
    expect(instance.x == 500);

    std::get<3>(tuple).test = false;
    expect(!instance.inner.test);
};
