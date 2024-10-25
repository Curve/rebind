#pragma once

#include "../member.hpp"

namespace rebind::utils
{
    namespace impl
    {
        template <typename T>
            requires std::is_aggregate_v<T>
        static constexpr auto member_names = []
        {
            constexpr auto size = std::tuple_size_v<decltype(rebind::members<T>)>;

            constexpr auto unpack = []<auto... Is>(std::index_sequence<Is...>)
            {
                constexpr auto members = rebind::members<T>;
                return std::array<std::string_view, sizeof...(Is)>{std::get<Is>(members).name...};
            };

            return unpack(std::make_index_sequence<size>());
        }();
    } // namespace impl

    template <typename T>
    static constexpr auto member_names = impl::member_names<T>;
} // namespace rebind::utils
