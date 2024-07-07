#pragma once

#include "name.hpp"

#include <tuple>
#include <optional>
#include <algorithm>

namespace rebind
{
    template <typename T>
        requires std::is_enum_v<T>
    static constexpr std::underlying_type_t<T> enum_find_min = -128;

    template <typename T>
        requires std::is_enum_v<T>
    static constexpr std::underlying_type_t<T> enum_find_max = 128;

    namespace impl
    {
        static constexpr std::string_view enum_start = "::";

        template <auto T>
            requires std::is_enum_v<decltype(T)>
        consteval auto enum_name()
        {
            constexpr auto name  = rebind::nttp_name<T>;
            constexpr auto delim = name.find(enum_start);

            if constexpr (delim == std::string_view::npos)
            {
                return std::string_view{};
            }
            else
            {
                return name.substr(delim + enum_start.size());
            }
        }

        template <typename T, auto I = enum_find_min<T>>
            requires std::is_enum_v<T>
        consteval auto enum_min()
        {
            if constexpr (I < enum_find_max<T> && enum_name<static_cast<T>(I)>().empty())
            {
                return enum_min<T, I + 1>();
            }
            else
            {
                return I;
            }
        }

        template <typename T, auto I = enum_find_max<T>>
            requires std::is_enum_v<T>
        consteval auto enum_max()
        {
            if constexpr (I > enum_find_min<T> && enum_name<static_cast<T>(I)>().empty())
            {
                return enum_max<T, I - 1>();
            }
            else
            {
                return I;
            }
        }

        template <typename T>
            requires std::is_enum_v<T>
        struct enum_field
        {
            T value;
            std::string_view name;
        };

        template <typename T>
            requires std::is_enum_v<T>
        consteval auto to_tuple()
        {
            constexpr auto min = enum_min<T>();
            constexpr auto max = enum_max<T>();

            auto make_tuple = []<auto V>(std::integral_constant<T, V>) -> std::tuple<enum_field<T>>
            {
                constexpr auto name = enum_name<V>();

                if constexpr (name.empty())
                {
                    return {};
                }
                else
                {
                    return {{V, name}};
                }
            };

            auto unpack = [&]<std::size_t... I>(std::index_sequence<I...>)
            {
                return std::tuple_cat(make_tuple(std::integral_constant<T, static_cast<T>(min + I)>{})...);
            };

            return unpack(std::make_index_sequence<max - min + 1>());
        }

        template <typename T>
            requires std::is_enum_v<T>
        constexpr auto enum_values()
        {
            constexpr auto convert = []<typename... V>(V &&...values)
            {
                return std::array{std::forward<V>(values)...};
            };

            return std::apply(convert, to_tuple<T>());
        }
    } // namespace impl

    template <auto T>
    static constexpr auto enum_name = impl::enum_name<T>();

    template <typename T>
    static constexpr auto enum_values = impl::enum_values<T>();

    template <typename T>
        requires std::is_enum_v<T>
    constexpr std::optional<impl::enum_field<T>> enum_value(T value)
    {
        constexpr auto fields = enum_values<T>;
        const auto rtn        = std::ranges::find_if(fields, [&](auto &&x) { return x.value == value; });

        if (rtn == fields.end())
        {
            return std::nullopt;
        }

        return *rtn;
    }
} // namespace rebind
