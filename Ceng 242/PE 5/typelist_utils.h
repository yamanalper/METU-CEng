#pragma once

template <typename... Ts>
struct List;

template <int index, typename T>
struct At;

template <typename Q, int index, typename T>
struct Find;

template <typename Q, int index, typename T>
struct Replace;

template <template <typename, typename...> typename F, typename Ret, typename T>
struct Map;

template <template <typename, typename...> typename F, typename Ret, typename T>
struct Filter;

template <typename T>
struct Size;

template <typename NewItem, typename T>
struct Append;

template <typename NewItem, typename T>
struct Prepend;

template <typename T, typename... Ts>
struct List<T, Ts...>
{
    using head = T;

    using tail = List<Ts...>;

    static constexpr auto size = Size<List<T, Ts...>>::value;

    template <int index>
    using at = typename At<index, List<T, Ts...>>::type;

    // questions

    template <typename Q>
    static constexpr auto find = Find<Q, 0, List<T, Ts...>>::value;

    template <int index, typename Q>
    using replace = typename Replace<Q, index, List<T, Ts...>>::type;

    template <typename Q>
    using append = typename Append<Q, List<T, Ts...>>::type;

    template <typename Q>
    using prepend = typename Prepend<Q, List<T, Ts...>>::type;

    template <template <typename, typename...> typename F>
    using map = typename Map<F, List<>, List<T, Ts...>>::type;

    template <template <typename, typename...> typename F>
    using filter = typename Filter<F, List<>, List<T, Ts...>>::type;
};

template <>
struct List<>
{
    // no head

    using tail = List<>; // empty list

    // no at

    static constexpr auto size = 0;

    static constexpr auto find = -1;

    // no replace

    template <typename Q>
    using append = List<Q>;

    template <typename Q>
    using prepend = List<Q>;

    template <template <typename, typename...> typename F>
    using map = List<>; // empty list

    template <template <typename, typename...> typename F>
    using filter = List<>; // empty list
};