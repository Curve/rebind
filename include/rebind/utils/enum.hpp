#pragma once

#include "../enum.hpp"

#include <optional>

namespace rebind::utils
{
    namespace impl
    {
        template <typename T>
        static constexpr auto enum_names = []
        {
            constexpr auto size = rebind::enum_values<T>.size();

            constexpr auto unpack = []<auto... Is>(std::index_sequence<Is...>)
            {
                constexpr auto values = rebind::enum_values<T>;
                return std::array<std::string_view, sizeof...(Is)>{rebind::enum_name<values[Is]>...};
            };

            return unpack(std::make_index_sequence<size>());
        }();

        template <typename T>
        constexpr std::optional<T> find_enum_value(std::string_view name)
        {
            constexpr auto values = rebind::enum_values<T>;
            constexpr auto names  = enum_names<T>;

            for (auto i = 0u; names.size() > i; ++i)
            {
                if (names[i] != name)
                {
                    continue;
                }

                return values[i];
            }

            return std::nullopt;
        }

        template <typename T>
        constexpr std::optional<std::string_view> find_enum_name(T value)
        {
            constexpr auto values = rebind::enum_values<T>;
            constexpr auto names  = enum_names<T>;

            for (auto i = 0u; names.size() > i; ++i)
            {
                if (values[i] != value)
                {
                    continue;
                }

                return names[i];
            }

            return std::nullopt;
        }
    } // namespace impl

    template <typename T>
        requires std::is_scoped_enum_v<T>
    static constexpr auto enum_names = impl::enum_names<T>;

    template <typename T>
        requires std::is_scoped_enum_v<T>
    constexpr auto find_enum_value(std::string_view name)
    {
        return impl::find_enum_value<T>(name);
    }

    template <typename T>
        requires std::is_scoped_enum_v<T>
    constexpr auto find_enum_name(T value)
    {
        return impl::find_enum_name(value);
    }
} // namespace rebind::utils
