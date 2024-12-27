#pragma once
#include "typelist_utils.h"
#include "typelist.h"

enum Direction
{
    LEFT = -1,
    STOP = 0,
    RIGHT = +1
};

template <typename OldState, typename Input, typename NewState,
          typename Output, Direction Move>
struct Rule
{
    typedef OldState old_state;
    typedef Input input;
    typedef NewState new_state;
    typedef Output output;
    static constexpr Direction direction = Move;
};

template <int n>
struct State
{
    static int constexpr value = n;
    static char const *name;
};

template <int n>
char const *State<n>::name = "unnamed";

struct QStart
{
    static constexpr int value = 0;
    static char constexpr const *name = "qstart";
};

struct QAccept
{
    static constexpr int value = -1;
    static constexpr char const *name = "qaccept";
};

struct QReject
{
    static constexpr int value = -2;
    static constexpr char const *name = "qreject";
};

template <int n>
struct Input
{
    static constexpr int value = n;
    static char const *name;
};

template <int n> char const *Input<n>::name = "unnamed";

using InputBlank = Input<-1>;
template<> char const* Input<-1>::name = "_";

template <typename InputTape, typename State, int Position>
struct Configuration
{
    using input_tape = InputTape;
    using state = State;
    static constexpr int position = Position;
};