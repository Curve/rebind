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

    rebind::visit(instance,
                  []<typename T, typename M>(T &value, const M &meta)
                  {
                      using type = M::type;

                      if constexpr (std::is_arithmetic_v<T>)
                      {
                          if (meta.name == "x")
                          {
                              expect(eq(value, 500));
                              expect(eq(meta.index, 0));
                              expect(std::is_same_v<type, int>);
                          }
                          else if (meta.name == "y")
                          {
                              expect(eq(value, 2));
                              expect(eq(meta.index, 1));
                              expect(std::is_same_v<type, float>);
                          }
                          else if (meta.name == "z")
                          {
                              expect(eq(value, 3));
                              expect(eq(meta.index, 2));
                              expect(std::is_same_v<type, double>);
                          }
                      }
                      else
                      {
                          expect(meta.name == "inner");

                          expect(eq(meta.index, 3));
                          expect(eq(value.test, false));
                          expect(std::is_aggregate_v<type>);
                      }
                  });
};
