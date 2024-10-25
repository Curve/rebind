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
            constexpr auto members = rebind::members<T>;

            const auto unpack = [&members]<auto... Is>(std::index_sequence<Is...>)
            {
                return std::array<std::string_view, sizeof...(Is)>{std::get<Is>(members).name...};
            };

            return unpack(std::make_index_sequence<std::tuple_size_v<decltype(members)>>());
        }();
    } // namespace impl

    template <typename T>
    static constexpr auto member_names = impl::member_names<T>;
} // namespace rebind::utils
