#pragma once
#include "turing-machine_utils.h"

template <typename A, typename B> struct is_same { static constexpr auto value = false; };
template <typename A> struct is_same<A, A>{ static constexpr auto value = true; };

template <bool C, typename = void> struct EnableIf;
template <typename Type> struct EnableIf<true, Type>{ using type = Type; };

template<typename Config, typename Transitions, typename = void>
   struct TransitionFunction { /* @TODO */
    using end_config = Configuration<InputTape, State, Position>;
    using end_input = InputTape;
    using end_state = State;
    static constexpr auto end_position = Position;
};

template <typename InputTape, typename State, int Position, typename Transitions>
struct TransitionFunction<Configuration<InputTape, State, Position>, Transitions,
                          typename EnableIf<is_same<State, QAccept>::value ||
                                            is_same<State, QReject>::value>::type>
{
   /* @TODO */
    using state = State;
    using input_tape = InputTape;
    static constexpr int position = Position;
    using current_input = typename input_tape::template at<position>;

    template <typename Rule>
    struct Matches {
        static constexpr bool value = is_same<typename Rule::old_state, state>::value &&
                                      is_same<typename Rule::input, current_input>::value;
    };

    using matching_rules = typename Transitions::template filter<Matches>;
    static constexpr bool has_matching_rule = matching_rules::size > 0;


    using rule = typename std::conditional<has_matching_rule,
                                           typename matching_rules::template at<0>,
                                           Rule<State, current_input, QReject, current_input, STOP>>::type;

    using new_input_tape = typename input_tape::template replace<position, typename rule::output>;
    using new_state = typename rule::new_state;
    static constexpr int new_position = position + rule::direction;

    using next_config = Configuration<new_input_tape, new_state, new_position>;
    using next_transition_function = TransitionFunction<next_config, Transitions>;

    using end_config = typename next_transition_function::end_config;
    using end_input = typename next_transition_function::end_input;
    using end_state = typename next_transition_function::end_state;
    static constexpr auto end_position = next_transition_function::end_position;
};