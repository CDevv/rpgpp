#ifndef jm4R_ENUM_VISITOR
#define jm4R_ENUM_VISITOR

#include <stdexcept>

namespace mj {

template <typename E, E... Values>
class enum_visitor;

template <typename T, typename E>
constexpr void visit(T& visitor, E value);

namespace detail {
    template <typename E, E... Values>
    class enum_visitor_invoker {

        using visitor_t = enum_visitor<E, Values...>;

        template <E Value>
        using enum_v = typename visitor_t::template enum_v<Value>;

        template <E v1, typename F>
        constexpr static bool visit_if_equal(F&& f, E v2)
        {
            if (v1 == v2) {
                f(enum_v<v1>{});
                return true;
            }
            return false;
        }

        constexpr static bool fail()
        {
            throw std::invalid_argument("Enum value not supported by provided visitor");
            return false;
        }

        template <typename F>
        constexpr static void visit(F&& f, E e)
        {
            (visit_if_equal<Values>(f, e) || ...) || fail();
        }

        template <typename T, typename E2>
        friend constexpr void ::mj::visit(T& visitor, E2 value);
    };

    template <typename E, E... Args>
    constexpr auto make_enum_visitor_invoker(const enum_visitor<E, Args...>&)
    {
        return enum_visitor_invoker<E, Args...>{};
    }
}

template <typename E, E... Values>
class enum_visitor {
public:
    template <E Value>
    class enum_v {
        constexpr static E value = Value;
    };
};

template <typename T, typename E>
constexpr void visit(T& visitor, E value)
{
    auto invoker = detail::make_enum_visitor_invoker(visitor);
    invoker.visit(visitor, value);
}
}

#endif //jm4R_ENUM_VISITOR
