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
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif
        template <typename T>
        struct dummy
        {
            static const T value;
        };

        template <typename T, std::size_t I>
        consteval decltype(auto) member_at()
        {
            return std::get<I>(to_tuple(dummy<T>::value));
        };
#ifdef __clang__
#pragma clang diagnostic pop
#endif

        template <typename T>
        struct pointer // Clang (<= 17) & MSVC workaround
        {
            const T *value;
        };

        template <typename T>
        consteval auto make_pointer(T &&value)
        {
            return pointer<std::remove_cvref_t<T>>{&value};
        }

        struct unmangle_me
        {
            int find_me;
        };

        static constexpr auto member_decorators =
            name_decorators<true>(mangled_name<make_pointer(member_at<unmangle_me, 0>())>(), "find_me");

        template <auto T>
        static constexpr auto unmangle_member = []
        {
            constexpr auto mangled    = mangled_name<T>();
            constexpr auto decorators = member_decorators;

            return unmangle(mangled, decorators);
        }();

        template <typename T>
        static constexpr auto members = []
        {
            constexpr auto arity = rebind::arity<T>;

            const auto unpack = []<auto I>(constant<I>)
            {
                constexpr auto &member = member_at<T, I>();
                using member_t         = std::remove_cvref_t<decltype(member)>;
                return rebind::member<member_t>{.index = I, .name = unmangle_member<make_pointer(member)>};
            };

            const auto unpack_sequence = [unpack]<auto... Is>(std::index_sequence<Is...>)
            {
                return std::make_tuple(unpack(constant<Is>{})...);
            };

            return unpack_sequence(std::make_index_sequence<arity>());
        }();

        template <auto P>
        static constexpr auto member_name = []
        {
            constexpr auto name = rebind::nttp_name<P>;
            constexpr auto type = []<typename T>(auto T::*)
            {
                return rebind::type_name<T>;
            }(P);

            return remove_type(name, type, "::");
        }();
    } // namespace impl

    template <typename T>
        requires std::is_aggregate_v<T>
    static constexpr auto members = impl::members<T>;

    template <auto P>
        requires std::is_member_pointer_v<decltype(P)>
    static constexpr auto member_name = impl::member_name<P>;
} // namespace rebind
