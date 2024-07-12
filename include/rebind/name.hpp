#pragma once

#include <string_view>
#include <type_traits>

#include <source_location>

namespace rebind
{
    namespace impl
    {
#if defined(__clang__) || defined(__GNUC__)
        static constexpr std::string_view nttp_start = "T = ";
        static constexpr std::string_view nttp_end   = "]";
#else
        static constexpr std::string_view nttp_start = "mangled_name<";
        static constexpr std::string_view nttp_end   = ">";
#endif

        static constexpr std::string_view type_start = "type_identity<";
#if defined(_MSC_VER) || defined(__clang__)
        static constexpr std::string_view type_end = ">{}";
#else
        static constexpr std::string_view type_end = ">()";
#endif

        static constexpr std::string_view enum_prefix = "enum ";

        template <auto T>
        consteval auto mangled_name()
        {
            return std::source_location::current().function_name();
        }

        template <typename T>
        consteval auto type_name()
        {
            constexpr std::string_view mangled = mangled_name<std::type_identity<T>{}>();

            constexpr auto start = mangled.substr(mangled.find(type_start) + type_start.size());
            constexpr auto end   = start.substr(0, start.rfind(type_end));

            if constexpr (end.starts_with(enum_prefix))
            {
                return end.substr(enum_prefix.size());
            }
            else
            {
                return end;
            }
        }

        template <auto T>
        consteval auto nttp_name()
        {
            constexpr std::string_view mangled = mangled_name<T>();

            constexpr auto start = mangled.substr(mangled.find(nttp_start) + nttp_start.size());
            constexpr auto end   = start.substr(0, start.rfind(nttp_end));

            return end;
        }
    } // namespace impl

    template <typename T>
    static constexpr auto type_name = impl::type_name<T>();

    template <auto T>
    static constexpr auto nttp_name = impl::nttp_name<T>();
} // namespace rebind
