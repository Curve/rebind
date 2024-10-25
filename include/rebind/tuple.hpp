#pragma once

#include <type_traits>
#include <tuple>

#include <boost/preprocessor/enum_params.hpp>
#include <boost/preprocessor/repeat_from_to.hpp>

namespace rebind
{
    namespace impl
    {
        struct universal
        {
            template <typename T>
            constexpr operator T() const;
        };

        template <typename T, typename... Us>
        consteval auto arity(Us &&...args)
        {
            if constexpr (requires { T{args...}; })
            {
                return arity<T>(args..., universal{});
            }

            return sizeof...(args) - 1;
        }

        template <typename T>
        constexpr auto to_tuple(T &value) // NOLINT(*-function-size)
        {
            constexpr auto N = arity<T>();

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
    } // namespace impl

    template <typename T>
        requires std::is_aggregate_v<T>
    static constexpr auto arity = impl::arity<T>();

    template <typename T>
        requires std::is_aggregate_v<T>
    constexpr auto to_tuple(T &value)
    {
        return impl::to_tuple(value);
    }
} // namespace rebind
