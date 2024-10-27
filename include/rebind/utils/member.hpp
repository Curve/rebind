#pragma once

#include "../member.hpp"

namespace rebind::utils
{
    template <typename V, typename C>
    struct member_pointer
    {
        using type = V C::*;

      public:
        using value_type = V;
        using class_type = C;

      public:
        type value;
        std::string_view name;
    };

    namespace impl
    {
        template <typename T>
        using member_types = decltype([]
        {
            constexpr auto arity = rebind::arity<T>;
            using members        = decltype(rebind::members<T>);

            constexpr auto unpack = []<auto... Is>(std::index_sequence<Is...>)
            {
                return std::type_identity<std::tuple<typename std::tuple_element_t<Is, members>::type...>>{};
            };

            return unpack(std::make_index_sequence<arity>());
        }())::type;

        template <typename T>
        static constexpr auto member_names = []
        {
            constexpr auto arity = rebind::arity<T>;

            constexpr auto unpack = []<auto... Is>(std::index_sequence<Is...>)
            {
                return std::array<std::string_view, sizeof...(Is)>{std::get<Is>(rebind::members<T>).name...};
            };

            return unpack(std::make_index_sequence<arity>());
        }();

        template <auto P>
        static constexpr auto member_info = []
        {
            constexpr auto unpack = []<typename V, typename C>(V C::*)
            {
                return member_pointer<V, C>{.value = P, .name = rebind::member_name<P>};
            };

            return unpack(P);
        }();
    } // namespace impl

    template <typename T>
        requires std::is_aggregate_v<T>
    using member_types = impl::member_types<T>;

    template <typename T>
        requires std::is_aggregate_v<T>
    static constexpr auto member_names = impl::member_names<T>;

    template <auto P>
        requires std::is_member_pointer_v<decltype(P)>
    static constexpr auto member_info = impl::member_info<P>;
} // namespace rebind::utils
