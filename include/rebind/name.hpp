#pragma once

#include <string_view>
#include <type_traits>

#include <source_location>

namespace rebind
{
    namespace impl
    {
        enum enum_ref
        {
        };

        template <auto T>
        consteval auto mangled_name()
        {
            return std::source_location::current().function_name();
        }

        constexpr auto unmangle_nttp_impl()
        {
            constexpr std::string_view mangled = mangled_name<true>();
            constexpr std::string_view to_find = "true";

            constexpr auto start = mangled.find(to_find);

            constexpr auto prefix = mangled.substr(0, start);
            constexpr auto suffix = mangled.substr(start + to_find.size());

            return std::make_pair(prefix, suffix);
        };

        constexpr auto unmangle_type_impl()
        {
            constexpr std::string_view mangled = mangled_name<std::type_identity<int>{}>();
            constexpr std::string_view to_find = "int";

            constexpr auto start = mangled.find(to_find);

            constexpr auto prefix = mangled.substr(0, start);
            constexpr auto suffix = mangled.substr(start + to_find.size());

            return std::make_pair(prefix, suffix);
        };

        constexpr auto unmangle_enum_impl()
        {
            constexpr std::string_view mangled = mangled_name<std::type_identity<enum_ref>{}>();
            constexpr std::string_view to_find = "enum_ref";

            constexpr auto occurrence = mangled.find(to_find);
            constexpr auto start      = mangled.substr(0, occurrence).find_last_of(" <") + 1;

            constexpr auto prefix = mangled.substr(0, start);
            constexpr auto suffix = mangled.substr(occurrence + to_find.size());

            return std::make_pair(prefix, suffix);
        };

        template <auto T, auto Func>
        consteval auto unmangle()
        {
            constexpr std::string_view mangled = mangled_name<T>();
            constexpr auto decorators          = Func();

            constexpr auto start = mangled.substr(mangled.rfind(decorators.first) + decorators.first.size());
            constexpr auto end   = start.substr(0, start.rfind(decorators.second));

            return end;
        }

        template <auto T>
        consteval auto nttp_name()
        {
            return unmangle<T, unmangle_nttp_impl>();
        }

        template <typename T>
        consteval auto type_name()
        {
            return unmangle<std::type_identity<T>{}, unmangle_type_impl>();
        }

        template <typename T>
            requires std::is_enum_v<T>
        consteval auto type_name()
        {
            return unmangle<std::type_identity<T>{}, unmangle_enum_impl>();
        }

        template <auto T, typename C>
        consteval auto extract_member()
        {
            constexpr std::string_view delim = "::";

            constexpr auto name = nttp_name<T>();
            constexpr auto type = type_name<C>();

            constexpr auto start = name.substr(name.rfind(type) + type.size());
            constexpr auto end   = start.rfind(delim);

            if (end == std::string_view::npos)
            {
                return std::string_view{};
            }
            else
            {
                return start.substr(end + delim.size());
            }
        }
    } // namespace impl

    template <typename T>
    static constexpr auto type_name = impl::type_name<T>();

    template <auto T>
    static constexpr auto nttp_name = impl::nttp_name<T>();
} // namespace rebind
