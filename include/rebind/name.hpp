#pragma once

#include "core.hpp"

namespace rebind
{
    namespace impl
    {
        template <auto T>
        consteval auto name_decorators(std::string_view needle)
        {
            constexpr auto mangled = mangled_name<T>();
            const auto start       = mangled.find(needle);

            const auto prefix = mangled.substr(0, start);
            const auto suffix = mangled.substr(start + needle.size());

            return std::make_pair(prefix, suffix);
        }

        static constexpr auto nttp_decorators = name_decorators<true>("true");
        static constexpr auto type_decorators = name_decorators<std::type_identity<int>{}>("int");

        template <auto T>
        static constexpr auto unmangle_nttp = []
        {
            constexpr auto mangled    = mangled_name<T>();
            constexpr auto decorators = nttp_decorators;

            return unmangle(mangled, decorators);
        }();

        template <typename T>
        static constexpr auto unmangle_type = []
        {
            constexpr auto mangled    = mangled_name<std::type_identity<T>{}>();
            constexpr auto decorators = type_decorators;

            return unmangle(mangled, decorators);
        }();

        template <auto T>
        static constexpr auto nttp_name = []
        {
            return unmangle_nttp<T>;
        }();

        template <typename T>
        static constexpr auto type_name = []
        {
            return remove_prefix(unmangle_type<T>, "enum ", "class ", "struct ");
        }();
    } // namespace impl

    template <typename T>
    static constexpr auto type_name = impl::type_name<T>;

    template <auto T>
    static constexpr auto nttp_name = impl::nttp_name<T>;
} // namespace rebind
