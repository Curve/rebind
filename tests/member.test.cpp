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

    constexpr auto names = rebind::member_names<simple>;
    expect(names.size() == 4);

    expect(names.at(0) == "x");
    expect(names.at(1) == "y");
    expect(names.at(2) == "z");
    expect(names.at(3) == "inner");

    constexpr auto field = rebind::inspect<simple, 1>;

    expect(field.num == 1);
    expect(field.name == "y");
    expect(std::is_same_v<decltype(field.type)::type, float>);

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

    rebind::visit(instance,
                  []<typename T>(T &value, const auto &meta)
                  {
                      using decayed   = std::decay_t<decltype(value)>;
                      using meta_type = typename decltype(meta.type)::type;

                      if constexpr (std::is_arithmetic_v<decayed>)
                      {
                          if (meta.name == "x")
                          {
                              expect(eq(value, 500));
                              expect(eq(meta.num, 0));
                              expect(std::is_same_v<meta_type, int>);
                          }
                          else if (meta.name == "y")
                          {
                              expect(eq(value, 2));
                              expect(eq(meta.num, 1));
                              expect(std::is_same_v<meta_type, float>);
                          }
                          else if (meta.name == "z")
                          {
                              expect(eq(value, 3));
                              expect(eq(meta.num, 2));
                              expect(std::is_same_v<meta_type, double>);
                          }
                      }
                      else
                      {
                          expect(meta.name == "inner");

                          expect(eq(meta.num, 3));
                          expect(eq(value.test, false));
                          expect(std::is_aggregate_v<meta_type>);
                      }
                  });
};
