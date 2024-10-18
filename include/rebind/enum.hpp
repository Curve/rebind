#pragma once

#include "name.hpp"

#include <array>
#include <tuple>
#include <cmath>

#include <cstdint>
#include <limits>

namespace rebind
{
    template <typename T>
        requires std::is_enum_v<T>
    static constexpr auto search_min = []
    {
        using underlying     = std::underlying_type_t<T>;
        constexpr auto limit = std::numeric_limits<underlying>::min();
        return static_cast<underlying>(std::max(static_cast<std::int64_t>(limit), static_cast<std::int64_t>(-128)));
    }();

    template <typename T>
        requires std::is_enum_v<T>
    static constexpr auto search_max = []
    {
        using underlying     = std::underlying_type_t<T>;
        constexpr auto limit = std::numeric_limits<underlying>::max();
        return static_cast<underlying>(std::min(static_cast<std::uint64_t>(limit), static_cast<std::uint64_t>(128)));
    }();

    namespace impl
    {
        template <auto T>
        concept is_enum_value = std::is_enum_v<decltype(T)>;

        template <auto T>
            requires is_enum_value<T>
        static constexpr auto enum_name = []
        {
            constexpr auto name = rebind::nttp_name<T>;
            constexpr auto type = rebind::type_name<decltype(T)>;

            return remove_type(name, type, "::");
        }();

        template <typename T, auto I, auto Max, typename State>
        consteval auto search_enum_values()
        {
            if constexpr (I < Max)
            {
                constexpr auto value = static_cast<T>(I);
                constexpr auto valid = !enum_name<value>.empty();

                if constexpr (valid)
                {
                    constexpr auto new_state = std::tuple_cat(State{}, std::tuple{constant<value>{}});
                    return search_enum_values<T, I + 1, Max, decltype(new_state)>();
                }
                else
                {
                    return search_enum_values<T, I + 1, Max, State>();
                }
            }
            else
            {
                return State{};
            }
        }

        template <typename T>
            requires std::is_enum_v<T>
        static constexpr auto enum_values = []
        {
            constexpr auto unpack = []<auto... Is>(std::tuple<constant<Is>...>)
            {
                return std::array<T, sizeof...(Is)>{static_cast<T>(Is)...};
            };

            return unpack(search_enum_values<T, search_min<T>, search_max<T>, std::tuple<>>());
        }();
    } // namespace impl

    template <auto T>
    static constexpr auto enum_name = impl::enum_name<T>;

    template <typename T>
    static constexpr auto enum_values = impl::enum_values<T>;
} // namespace rebind
