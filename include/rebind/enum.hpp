#pragma once

#include "name.hpp"

#include <array>
#include <cmath>

#include <cstdint>
#include <limits>

namespace rebind
{
    template <typename T>
        requires std::is_scoped_enum_v<T>
    static constexpr auto search_min = []
    {
        using underlying     = std::underlying_type_t<T>;
        constexpr auto limit = std::numeric_limits<underlying>::min();
        return static_cast<underlying>(std::max(static_cast<std::intmax_t>(limit), static_cast<std::intmax_t>(-64)));
    }();

    template <typename T>
        requires std::is_scoped_enum_v<T>
    static constexpr auto search_max = []
    {
        using underlying     = std::underlying_type_t<T>;
        constexpr auto limit = std::numeric_limits<underlying>::max();
        return static_cast<underlying>(std::min(static_cast<std::intmax_t>(limit), static_cast<std::intmax_t>(64)));
    }();

    namespace impl
    {
        template <auto T>
        concept is_scoped_enum_value = std::is_scoped_enum_v<decltype(T)>;

        template <auto T>
        static constexpr auto enum_name = []
        {
            constexpr auto name = rebind::nttp_name<T>;
            constexpr auto type = rebind::type_name<decltype(T)>;

            return remove_type(name, type, "::");
        }();

        template <typename T, auto I, auto Max, auto... State>
        consteval auto search_enum_values()
        {
            if constexpr (I < Max)
            {
                constexpr auto value = static_cast<T>(I);
                constexpr auto valid = !enum_name<value>.empty();

                if constexpr (valid)
                {
                    return search_enum_values<T, I + 1, Max, State..., value>();
                }
                else
                {
                    return search_enum_values<T, I + 1, Max, State...>();
                }
            }
            else
            {
                return std::array<T, sizeof...(State)>{State...};
            }
        }

        template <typename T>
        static constexpr auto enum_values = []
        {
            return search_enum_values<T, search_min<T>, search_max<T>>();
        }();
    } // namespace impl

    template <auto T>
        requires impl::is_scoped_enum_value<T>
    static constexpr auto enum_name = impl::enum_name<T>;

    template <typename T>
        requires std::is_scoped_enum_v<T>
    static constexpr auto enum_values = impl::enum_values<T>;
} // namespace rebind
