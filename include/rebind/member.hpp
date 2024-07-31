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
        template <typename T>
        extern T external;

        template <typename T>
        struct pointer
        {
            const T *value;
        };

        struct some_ref
        {
            int find_me;
        };

        template <typename T>
        consteval auto max(const T &first, const T &second)
        {
            if (first == std::string_view::npos)
            {
                return second;
            }

            if (second == std::string_view::npos)
            {
                return first;
            }

            return first > second ? first : second;
        }

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Weverything"
#endif
        static constexpr auto unmangle_member_impl = []()
        {
            constexpr auto &ref                = get<0>(external<some_ref>);
            constexpr std::string_view mangled = rebind::impl::mangled_name<pointer{&ref}>();

            static constexpr std::string_view field    = "find_me";
            static constexpr std::string_view instance = "external";
            static constexpr std::string_view type     = "some_ref";

            constexpr auto field_occurrence    = mangled.find(field);
            constexpr auto type_occurrence     = mangled.rfind(type, field_occurrence);
            constexpr auto instance_occurrence = mangled.rfind(instance, field_occurrence);

            constexpr auto start = max(instance_occurrence, type_occurrence + type.size());

            constexpr auto prefix = mangled.substr(start, field_occurrence - start);
            constexpr auto suffix = mangled.substr(field_occurrence + field.size());

            return std::make_pair(prefix, suffix);
        };

        template <pointer T>
        consteval auto member_name()
        {
            return unmangle<T, unmangle_member_impl>();
        }

        template <typename T, std::size_t I>
            requires std::is_aggregate_v<T>
        consteval auto inspect()
        {
            static constexpr auto &field = get<I>(external<T>);
            using field_t                = std::decay_t<decltype(field)>;

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
            static constexpr auto unpack = []<std::size_t... I>(std::index_sequence<I...>)
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
