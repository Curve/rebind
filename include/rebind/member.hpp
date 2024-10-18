#pragma once

#include "name.hpp"
#include "tuple.hpp"

namespace rebind
{
    template <typename T>
    struct member
    {
        using type = T;

      public:
        std::size_t index;
        std::string_view name;
    };

    namespace impl
    {
        template <typename T>
        extern T external;

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif
        template <typename T, std::size_t I>
        static consteval decltype(auto) member_at()
        {
            return std::get<I>(to_tuple(external<T>));
        };

        template <typename T>
        static constexpr auto member_count = []
        {
            return std::tuple_size_v<decltype(to_tuple(external<T>))>;
        }();
#ifdef __clang__
#pragma clang diagnostic pop
#endif

        struct some_ref
        {
            int find_me;
        };

        static constexpr auto external_decorators = []
        {
            constexpr auto member  = &member_at<some_ref, 0>();
            constexpr auto mangled = mangled_name<member>();

            constexpr std::string_view instance = "external";
            constexpr std::string_view name     = "find_me";

            constexpr auto start = mangled.find(instance);
            constexpr auto end   = mangled.rfind(name);

            constexpr auto prefix = mangled.substr(0, start + instance.size());
            constexpr auto suffix = mangled.substr(end + name.size());

            return std::make_pair(prefix, suffix);
        }();

        template <auto T>
        static constexpr auto unmangle_external = []
        {
            constexpr auto mangled    = mangled_name<T>();
            constexpr auto decorators = external_decorators;

            return unmangle(mangled, decorators);
        }();

        static constexpr auto member_decorators = []
        {
            constexpr auto member  = &member_at<some_ref, 0>();
            constexpr auto mangled = unmangle_external<member>;

            constexpr std::string_view type = "some_ref";
            constexpr std::string_view name = "find_me";

            constexpr auto type_pos = mangled.rfind(type);
            constexpr auto name_pos = mangled.rfind(name);

            constexpr auto start = type_pos == std::string_view::npos ? 0 : type_pos + type.size();

            constexpr auto prefix = mangled.substr(start, name_pos - start);
            constexpr auto suffix = mangled.substr(name_pos + name.size());

            return std::make_pair(prefix, suffix);
        }();

        template <auto T>
        static constexpr auto unmangle_member = []
        {
            constexpr auto mangled    = unmangle_external<T>;
            constexpr auto decorators = member_decorators;

            return unmangle(mangled, decorators);
        }();

        template <typename T>
            requires std::is_aggregate_v<T>
        static constexpr auto members = []
        {
            constexpr auto unpack = []<auto... Is>(std::index_sequence<Is...>)
            {
                auto unpack_at = []<auto I>(constant<I>)
                {
                    constexpr auto &member = member_at<T, I>();

                    return rebind::member<std::remove_reference_t<decltype(member)>>{
                        .index = I,
                        .name  = unmangle_member<&member>,
                    };
                };

                return std::make_tuple(unpack_at(constant<Is>{})...);
            };

            return unpack(std::make_index_sequence<member_count<T>>{});
        }();

        template <auto T, typename C, typename V>
            requires std::is_member_pointer_v<decltype(T)>
        consteval auto member_name(V C::*)
        {
            constexpr auto name = rebind::nttp_name<T>;
            constexpr auto type = rebind::type_name<C>;

            return remove_type(name, type, "::");
        }
    } // namespace impl

    template <typename T>
    static constexpr auto members = impl::members<T>;

    template <auto T>
    static constexpr auto member_name = impl::member_name<T>(T);
} // namespace rebind
