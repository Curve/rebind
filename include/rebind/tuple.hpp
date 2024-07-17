#pragma once

#include <type_traits>
#include <cstddef>
#include <tuple>

#include <boost/preprocessor/repeat_from_to.hpp>
#include <boost/preprocessor/enum_params.hpp>

namespace rebind
{
    namespace impl
    {
        struct universal
        {
            template <typename Self, typename T>
            constexpr operator T(this Self &&);
        };

        template <typename T, typename... Us>
            requires std::is_aggregate_v<T>
        consteval auto arity(Us &&...args)
        {
            if constexpr (!requires { T{std::forward<Us>(args)...}; })
            {
                return sizeof...(args) - 1;
            }
            else
            {
                return arity<T>(std::forward<Us>(args)..., universal{});
            }
        }
    } // namespace impl

    template <typename T>
    static constexpr auto arity = impl::arity<T>();

    template <typename T>
        requires std::is_aggregate_v<T>
    constexpr auto to_tuple(T &value) // NOLINT(*-function-size)
    {
        static constexpr auto N = arity<T>;

// NOLINTNEXTLINE
#define REBIND_GET_N(_, _N, __)                                                                                        \
    if constexpr (N == (_N))                                                                                           \
    {                                                                                                                  \
        auto &[BOOST_PP_ENUM_PARAMS(_N, mem_)] = value;                                                                \
        return std::tie(BOOST_PP_ENUM_PARAMS(_N, mem_));                                                               \
    }

#if defined(_MSC_VER)
#define REBIND_GET_LIMIT 128
#else
#define REBIND_GET_LIMIT BOOST_PP_LIMIT_MAG
#endif

        BOOST_PP_REPEAT_FROM_TO(1, REBIND_GET_LIMIT, REBIND_GET_N, 0);

#undef REBIND_GET_N
#undef REBIND_GET_LIMIT
    }

    template <std::size_t I, typename T>
        requires std::is_aggregate_v<T>
    constexpr decltype(auto) get(T &value)
    {
        return std::get<I>(to_tuple(value));
    }
} // namespace rebind
