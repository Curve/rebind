#pragma once

#include "core.hpp"

namespace rebind
{
    namespace impl
    {
        template <bool Partial = false>
        consteval auto name_decorators(std::string_view mangled, std::string_view needle)
        {
            const auto start = mangled.find(needle);

            const auto prefix = Partial ? mangled.substr(start - 1, 1) : mangled.substr(0, start);
            const auto suffix = mangled.substr(start + needle.size());

            return std::make_pair(prefix, suffix);
        }

        static constexpr auto nttp_decorators = name_decorators(mangled_name<true>(), "true");
        static constexpr auto type_decorators = name_decorators(mangled_name<int>(), "int");

        template <auto T>
        static constexpr auto nttp_name = []
        {
            return unmangle(mangled_name<T>(), nttp_decorators);
        }();

        template <typename T>
        static constexpr auto type_name = []
        {
            return remove_prefix(unmangle(mangled_name<T>(), type_decorators), "enum ", "class ", "struct ");
        }();
    } // namespace impl

    template <typename T>
    static constexpr auto type_name = impl::type_name<T>;

    template <auto T>
    static constexpr auto nttp_name = impl::nttp_name<T>;
} // namespace rebind
