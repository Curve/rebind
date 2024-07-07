#pragma once

#include "name.hpp"
#include "tuple.hpp"

namespace rebind
{
    template <typename T, std::size_t I>
    struct member
    {
        std::string_view name;

      public:
        using type                  = T;
        static constexpr auto index = I;
    };

    namespace impl
    {
        template <std::size_t I>
        using index_constant = std::integral_constant<std::size_t, I>;

        template <typename T>
        extern T external;

        template <typename T>
        struct pointer
        {
            const T *value;
        };

#if defined(__clang__)
        static constexpr std::string_view member_start = "external.";
        static constexpr std::string_view member_end   = "}";
#elif defined(_MSC_VER)
        static constexpr std::string_view member_start = "->";
        static constexpr std::string_view member_end   = "}";
#else
        static constexpr std::string_view member_start = "::";
        static constexpr std::string_view member_end   = "))}";
#endif

        template <pointer T>
        consteval auto member_name()
        {
            constexpr auto name = rebind::nttp_name<T>;

            constexpr auto start = name.substr(name.rfind(member_start) + member_start.size());
            constexpr auto end   = start.substr(0, start.rfind(member_end));

            return end;
        }

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif
        template <typename T, std::size_t I>
            requires std::is_aggregate_v<T>
        consteval auto inspect()
        {
            constexpr auto &field = get<I>(external<T>);
            using field_t         = std::decay_t<decltype(field)>;

            return member<field_t, I>{member_name<pointer{&field}>()};
        }
#ifdef __clang__
#pragma clang diagnostic pop
#endif

        template <typename... Ts>
        consteval auto make_array(Ts &&...values)
        {
            // We need this helper because msvc is retarded. The following issue seems related but is not quite the
            // same: https://developercommunity.visualstudio.com/t/Error-calling-consteval-function-from-an/1669482

            return std::array{std::forward<Ts>(values)...};
        }

        template <typename T>
        consteval auto member_names()
        {
            auto unpack = []<std::size_t... I>(std::index_sequence<I...>)
            {
                return make_array(inspect<T, I>().name...);
            };

            return unpack(std::make_index_sequence<rebind::arity<T>>());
        }
    } // namespace impl

    template <typename T, std::size_t I>
    static constexpr auto inspect = impl::inspect<T, I>();

    template <typename T>
    static constexpr auto member_names = impl::member_names<T>();

    template <typename T, typename Fn>
        requires std::is_aggregate_v<T>
    constexpr auto visit(T &value, Fn &&callback)
    {
        auto unpack = [&]<std::size_t... I>(std::index_sequence<I...>)
        {
            (callback(get<I>(value), inspect<T, I>), ...);
        };

        return unpack(std::make_index_sequence<arity<T>>());
    }
} // namespace rebind
