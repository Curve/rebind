#pragma once

#include "../name.hpp"

namespace rebind::utils
{
    namespace impl
    {
        consteval auto remove_namespace(std::string_view name)
        {
            const auto start = name.rfind("::");

            if (start == std::string_view::npos)
            {
                return name;
            }
            else
            {
                return name.substr(start + 2);
            }
        }

        template <typename T>
        static constexpr auto pure_name = []
        {
            return remove_namespace(rebind::type_name<T>);
        }();
    } // namespace impl

    template <typename T>
    static constexpr auto pure_name = impl::pure_name<T>;
} // namespace rebind::utils
