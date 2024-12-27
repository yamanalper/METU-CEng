#pragma once
#include "typelist_utils.h"

template<typename ...Ts>
struct Size<List<Ts...>> {
    static constexpr auto value = sizeof...(Ts);
};

// @brief base case -- Get the 0th element of a list
template<typename T, typename... Ts>
struct At<0, List<T, Ts...>> {
    using type = T;
};

// @brief inductive case -- Get the Nth element of a list
template<int index, typename T, typename... Ts>
struct At<index, List<T, Ts...>> {
    static_assert(index > 0, "index cannot be negative");
    using type = typename At<index - 1, List<Ts...>>::type;
};

struct NotImpl;
#define NOT_IMPL NotImpl



template<typename Q, int index, typename T, typename... Ts>
struct Find<Q, index, List<T, Ts...>> {
    static constexpr int value = Find<Q, index + 1, List<Ts...>>::value;
};

template<typename Q, int index, typename... Ts>
struct Find<Q, index, List<Q, Ts...>> {
    static constexpr int value = index;
};

template<typename Q, int index>
struct Find<Q, index, List<>> {
    static constexpr int value = -1;
};



template<typename Q, int index, typename T, typename... Ts>
struct Replace<Q, index, List<T, Ts...>> {
    using type = List<T, typename Replace<Q, index - 1, List<Ts...>>::type>;
};

template<typename Q, typename T, typename... Ts>
struct Replace<Q, 0, List<T, Ts...>> {
    using type = List<Q, Ts...>;
};

template<typename Q, int index>
struct Replace<Q, index, List<>> {
    using type = List<>;
};



template<typename NewItem, typename... Ts>
struct Append<NewItem, List<Ts...>> {
    using type = List<Ts..., NewItem>;
};



template<typename NewItem, typename... Ts>
struct Prepend<NewItem, List<Ts...>> {
    using type = List<NewItem, Ts...>;
};

template<bool B, typename T, typename F>
struct conditional {
    using type = T;
};

template<typename T, typename F>
struct conditional<false, T, F> {
    using type = F;
};



template<template<typename> typename F, typename Ret, typename T, typename... Ts>
struct Map<F, Ret, List<T, Ts...>> {
    using type = typename Map<F, typename Append<typename F<T>::type, Ret>::type, List<Ts...>>::type;
};

template<template<typename> typename F, typename Ret>
struct Map<F, Ret, List<>> {
    using type = Ret;
};



template<template<typename> typename F, typename Ret, typename T, typename... Ts>
struct Filter<F, Ret, List<T, Ts...>> {
    using type = typename conditional<
        F<T>::value,
        typename Filter<F, typename Append<T, Ret>::type, List<Ts...>>::type,
        typename Filter<F, Ret, List<Ts...>>::type
    >::type;
};

template<template<typename> typename F, typename Ret>
struct Filter<F, Ret, List<>> {
    using type = Ret;
};