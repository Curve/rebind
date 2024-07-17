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

        static constexpr auto unmangle_nttp_impl = []()
        {
            static constexpr std::string_view mangled = mangled_name<true>();
            static constexpr std::string_view to_find = "true";

            static constexpr auto start = mangled.find(to_find);

            static constexpr auto prefix = mangled.substr(0, start);
            static constexpr auto suffix = mangled.substr(start + to_find.size());

            return std::make_pair(prefix, suffix);
        };

        static constexpr auto unmangle_type_impl = []()
        {
            static constexpr std::string_view mangled = mangled_name<std::type_identity<int>{}>();
            static constexpr std::string_view to_find = "int";

            static constexpr auto start = mangled.find(to_find);

            static constexpr auto prefix = mangled.substr(0, start);
            static constexpr auto suffix = mangled.substr(start + to_find.size());

            return std::make_pair(prefix, suffix);
        };

        static constexpr auto unmangle_enum_impl = []()
        {
            static constexpr std::string_view mangled = mangled_name<std::type_identity<rebind::impl::enum_ref>{}>();
            static constexpr std::string_view to_find = "enum_ref";

            static constexpr auto occurrence = mangled.find(to_find);
            static constexpr auto start      = mangled.substr(0, occurrence).find_last_of(" <") + 1;

            static constexpr auto prefix = mangled.substr(0, start);
            static constexpr auto suffix = mangled.substr(occurrence + to_find.size());

            return std::make_pair(prefix, suffix);
        };

        template <auto T, auto Func>
        consteval auto unmangle()
        {
            static constexpr std::string_view mangled = mangled_name<T>();
            static constexpr auto decorators          = Func();

            static constexpr auto start = mangled.substr(mangled.rfind(decorators.first) + decorators.first.size());
            static constexpr auto end   = start.substr(0, start.rfind(decorators.second));

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
    } // namespace impl

    template <typename T>
    static constexpr auto type_name = impl::type_name<T>();

    template <auto T>
    static constexpr auto nttp_name = impl::nttp_name<T>();
} // namespace rebind
