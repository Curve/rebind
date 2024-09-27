#pragma once

#include "name.hpp"

#include <cmath>
#include <limits>

#include <array>
#include <optional>
#include <algorithm>

namespace rebind
{
    template <typename T>
        requires std::is_enum_v<T>
    static constexpr std::underlying_type_t<T> enum_find_min = 0;

    template <typename T>
        requires std::is_enum_v<T>
    static constexpr std::underlying_type_t<T> enum_find_max =
        std::min(std::numeric_limits<std::underlying_type_t<T>>::max(), static_cast<std::underlying_type_t<T>>(128));

    namespace impl
    {
        static constexpr std::string_view enum_start = "::";

        template <typename T>
            requires std::is_enum_v<T>
        struct enum_field
        {
            T value;
            std::string_view name;
        };

        template <auto T>
            requires std::is_enum_v<decltype(T)>
        consteval auto enum_name()
        {
            constexpr auto name = rebind::nttp_name<T>;
            constexpr auto type = rebind::type_name<decltype(T)>;

            constexpr auto start = name.substr(name.rfind(type) + type.size());
            constexpr auto end   = start.rfind(enum_start);

            if constexpr (end == std::string_view::npos)
            {
                return std::string_view{};
            }
            else
            {
                return start.substr(end + enum_start.size());
            }
        }

        template <auto T>
            requires std::is_enum_v<decltype(T)>
        consteval auto is_valid()
        {
            return !enum_name<T>().empty();
        }

        template <typename T, auto I = enum_find_min<T>, auto R = 0>
            requires std::is_enum_v<T>
        consteval std::size_t valid_indices()
        {
            if constexpr (I < enum_find_max<T>)
            {
                if constexpr (is_valid<static_cast<T>(I)>())
                {
                    return valid_indices<T, I + 1, R + 1>();
                }
                else
                {
                    return valid_indices<T, I + 1, R>();
                }
            }

            return R;
        }

        template <typename T, auto I = enum_find_min<T>, auto C = 0>
            requires std::is_enum_v<T>
        consteval void populate(auto &rtn)
        {
            constexpr auto value = static_cast<T>(I);

            if constexpr (I < enum_find_max<T>)
            {
                if constexpr (is_valid<value>())
                {
                    rtn[I] = enum_field<T>{
                        .value = value,
                        .name  = enum_name<value>(),
                    };

                    return populate<T, I + 1, C + 1>(rtn);
                }
                else
                {
                    return populate<T, I + 1, C>(rtn);
                }
            }
        }

        template <typename T>
            requires std::is_enum_v<T>
        consteval auto enum_fields()
        {
            std::array<enum_field<T>, valid_indices<T>()> rtn;
            populate<T>(rtn);
            return rtn;
        }
    } // namespace impl

    template <auto T>
    static constexpr auto enum_name = impl::enum_name<T>();

    template <typename T>
    static constexpr auto enum_fields = impl::enum_fields<T>();

    template <typename T>
        requires std::is_enum_v<T>
    constexpr std::optional<std::string_view> find_enum_name(T value)
    {
        constexpr auto fields = enum_fields<T>;
        const auto rtn        = std::ranges::find_if(fields, [&](auto &&x) { return x.value == value; });

        if (rtn == fields.end())
        {
            return std::nullopt;
        }

        return rtn->name;
    }

    template <typename T>
        requires std::is_enum_v<T>
    constexpr std::optional<T> find_enum_value(std::string_view name)
    {
        constexpr auto fields = enum_fields<T>;
        const auto rtn        = std::ranges::find_if(fields, [&](auto &&x) { return x.name == name; });

        if (rtn == fields.end())
        {
            return std::nullopt;
        }

        return rtn->value;
    }
} // namespace rebind
