//
// Created by alvinmax on 07.03.19.
//

#ifndef BIND_BIND_H
#define BIND_BIND_H

#include <tuple>

template<typename T, T ... values>
struct integer_sequence {
};

template<typename T, typename Seq, T arg>
struct append;

template<typename T, T ... values, T arg>
struct append<T, integer_sequence<T, values...>, arg> {
    typedef integer_sequence<T, values..., arg> type;
};

template<typename T, T N, typename Q = void>
struct make_integer_sequence {
    typedef typename append<T, typename make_integer_sequence<T, N - 1>::type, N - 1>::type type;
};

template<typename T, T N>
struct make_integer_sequence<T, N, typename std::enable_if<N == 0>::type> {
    typedef integer_sequence<T> type;
};

template<typename T, typename U>
struct merge;

template<int... N1, int... N2>
struct merge<integer_sequence<int, N1...>, integer_sequence<int, N2...>> {
    typedef integer_sequence<int, N1..., N2...> value;
};

template<int N>
struct placeholder {
};

template<typename F, typename ... As>
struct bind_t;

constexpr placeholder<1> _1;
constexpr placeholder<2> _2;
constexpr placeholder<3> _3;

//---------------------
template<typename A>
struct G {
    template<typename AA>
    G(AA &&aa) : a(std::forward<AA>(aa)) {}

    template<typename ... Bs>
    A operator()(Bs &&...) {
        return static_cast<A>(a);
    }

    std::decay_t<A> a;
};

template<int N>
struct G<placeholder<N>> {
    G(placeholder<N>) {}

    template<typename B, typename ... Bs>
    decltype(auto) operator()(B &&, Bs &&... bs) {
        G<placeholder<N - 1>> next{placeholder<N - 1>()};
        return next(std::forward<Bs>(bs)...);
    }
};

template<>
struct G<placeholder<1>> {
    G(placeholder<1>) {}

    template<typename B1, typename ... Bs>
    decltype(auto) operator()(B1 &&b1, Bs &&...) {
        return std::forward<B1>(b1);
    }
};

template<typename F, typename ... As>
struct G<bind_t<F, As...>> {
    G(bind_t<F, As...> &&b) : fun(std::move(b)) {}

    template<typename ... Bs>
    decltype(auto) operator()(Bs &&... bs) {
        return fun(std::forward<Bs>(bs)...);
    }

    bind_t<F, As...> fun;
};

// type_converter_t

template<typename T, bool Is_unique>
struct type_converter {
    typedef T &type;
};
template<typename T>
struct type_converter<T, true> {
    typedef T &&type;
};

template<typename T, bool Is_unique>
using type_converter_t = typename type_converter<T, Is_unique>::type;

//  unique_checker_v

template<int I, typename S>
struct unique_checker;

template<int I, int CURRENT, int... TAIL>
struct unique_checker<I, integer_sequence<int, CURRENT, TAIL...>> {
    static const int value = (I == CURRENT) + unique_checker<I, integer_sequence<int, TAIL...>>::value;
};


template<int I>
struct unique_checker<I, integer_sequence<int>> {
    static const int value = 0;
};


template<int I, typename S>
constexpr bool unique_checker_v = unique_checker<I, S>::value == 1;

// numbers_of

template<typename T>
struct number_of {
    typedef integer_sequence<int> value;
};

template<int N>
struct number_of<placeholder<N>> {
    typedef integer_sequence<int, N> value;
};

template<typename ... S>
struct numbers_of;

template<typename F, typename ... As>
struct number_of<bind_t<F, As...>> {
    typedef typename numbers_of<As...>::value value;
};

template<typename T, typename ... TAIL>
struct numbers_of<T, TAIL...> {
    typedef typename merge<typename number_of<T>::value, typename numbers_of<TAIL...>::value>::value value;
};

template<>
struct numbers_of<> {
    typedef integer_sequence<int> value;
};

template<typename ... T>
using numbers_of_t = typename numbers_of<T...>::value;

// -------------------------------------------

template<typename T>
struct wrapper {
    typedef T type;
};
template<int N>
struct wrapper<placeholder<N> &> {
    typedef placeholder<N> type;
};
template<int N>
struct wrapper<placeholder<N> &&> {
    typedef placeholder<N> type;
};

template<typename F, typename ... As>
struct wrapper<bind_t<F, As...> &> {
    typedef bind_t<F, As...> type;
};

template<typename F, typename ... As>
struct wrapper<bind_t<F, As...> &&> {
    typedef bind_t<F, As...> type;
};

template<typename T>
using wrapper_t = typename wrapper<T>::type;

template<typename F, typename ... As>
struct bind_t {
    template<typename FF, typename ... AAs>
    bind_t(FF &&ff, AAs &&... as)
            : f(std::forward<FF>(ff)), gs(std::forward<AAs>(as)...) {}

    template<typename ... Bs>
    decltype(auto) operator()(Bs &&... bs) {
        return call(typename make_integer_sequence<int, sizeof...(As)>::type(),
                    typename make_integer_sequence<int, sizeof...(Bs)>::type(),
                    std::forward<Bs>(bs)...);
    }

private:
    template<int ... ks, int ... i, typename ... Bs>
    decltype(auto)
    call(integer_sequence<int, ks...> a, integer_sequence<int, i...> b,
         Bs &&... bs) {
        return f(std::get<ks>(gs)(
                static_cast<type_converter_t<Bs, unique_checker_v<i + 1, numbers_of_t<As...>>>>(bs)...)...);
    }

    F f;
    std::tuple<G<As>...> gs;
};


template<typename F, typename ... As>
decltype(auto) bind(F &&f, As &&... as) {
    return bind_t<std::decay_t<F>, wrapper_t<std::decay_t<As> &>...>(std::forward<F>(f), std::forward<As>(as)...);
}

template<typename F, typename ... As>
decltype(auto) call_once_bind(F &&f, As &&... as) {
    return bind_t<std::decay_t<F>, wrapper_t<std::decay_t<As> &&>...>(std::forward<F>(f), std::forward<As>(as)...);
}

#endif //BIND_BIND_H
