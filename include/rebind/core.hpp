#pragma once

#include <string_view>
#include <source_location>

namespace rebind::impl
{
    template <auto I>
    using constant = std::integral_constant<decltype(I), I>;

    template <typename T>
    consteval std::string_view mangled_name()
    {
        return std::source_location::current().function_name();
    }

    template <auto T>
    consteval std::string_view mangled_name()
    {
        return std::source_location::current().function_name();
    }

    template <typename U>
    constexpr auto unmangle(std::string_view name, U decorators)
    {
        const auto start = name.substr(0, name.rfind(decorators.second));
        const auto end   = start.substr(start.rfind(decorators.first) + decorators.first.size());

        return end;
    }

    template <typename... Ts>
    constexpr auto remove_prefix(std::string_view name, const Ts &...prefixes)
    {
        const auto has_prefix = (name.starts_with(prefixes) || ...);

        if (!has_prefix)
        {
            return name;
        }

        return name.substr(name.find(' ') + 1);
    }

    constexpr auto remove_type(std::string_view name, std::string_view type, std::string_view delim)
    {
        const auto start = name.substr(name.rfind(type) + type.size());
        const auto end   = start.find(delim);

        if (end == std::string_view::npos)
        {
            return std::string_view{};
        }
        else
        {
            return start.substr(end + delim.size());
        }
    }
} // namespace rebind::impl
