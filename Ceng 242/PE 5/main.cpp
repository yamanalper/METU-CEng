#include "typelist_utils.h"
#include "typelist.h"
#include "turing-machine_utils.h"
#include "turing-machine.h"
#include <type_traits>
#include <iostream>


template <typename InputTape, typename Transitions>
struct TuringMachine
{
    using input_tape = InputTape;
    using transitions = Transitions;
    using start_state = QStart;
    using start_config = Configuration<input_tape, start_state, 0>;
    using transition_function = TransitionFunction<start_config, Transitions>;

    using end_config = typename transition_function::end_config;
    using end_input = typename transition_function::end_input;
    using end_state = typename transition_function::end_state;
    static constexpr auto end_position = transition_function::end_position;
};



int main() { 
    using L3 = List<int, double, char>;

    static_assert(std::is_same<L3::append<float>, List<int, double, char, float>>::value);

    static_assert(std::is_same<L3::prepend<float>, List<float, int, double, char>>::value);

    using L4 = List<float, char, double>;

    // static_assert(std::is_same<L3::concat<L4>, List<int, double, char, float, char, double>>::value);

    static_assert(std::is_same<List<int, float, char>::replace<1, double>, List<int, double, char>>::value);

    static_assert(std::is_same<List<int, float, char>::replace<0, double>, List<double, float, char>>::value);

    using f = L3::map<std::add_pointer>;
    using g = List<int*, double*, char*>;
    // map example
    static_assert(std::is_same<f, g>::value);
    //static_assert(std::is_same<f::at<0>::type, g::at<0>>::value);
    //static_assert(std::is_same<f::at<1>::type, g::at<1>>::value);
    //static_assert(std::is_same<f::at<2>::type, g::at<2>>::value);


    using h = L3::filter<std::is_floating_point>;
    static_assert(std::is_same<h, List<double>>::value);
    /*
    DEF_INPUT(1, x);
    DEF_INPUT(2, x_mark);
    DEF_INPUT(3, split);

    DEF_STATE(1, Q_find_blank);
    DEF_STATE(2, Q_go_back);

    using rules = List<
        Rule<QStart, x, Q_find_blank, x_mark, RIGHT>,
        Rule<Q_find_blank, x, Q_find_blank, x, RIGHT>,
        Rule<Q_find_blank, split, Q_find_blank, split, RIGHT>,
        Rule<Q_find_blank, InputBlank, Q_go_back, x, LEFT>,
        Rule<Q_go_back, x, Q_go_back, x, LEFT>,
        Rule<Q_go_back, split, Q_go_back, split, LEFT>,
        Rule<Q_go_back, x_mark, QStart, x, RIGHT>,
        Rule<QStart, split, QAccept, split, LEFT>
    >;

    using double_it = TuringMachine<List<x, x, x, x, split>, rules>;
    using tm_true = List<x, x, x, x, split, x, x, x, x>;

    using tm_res =  typename double_it::end_input;
    using tm_state = typename double_it::end_state;

    //static_assert(std::is_same<tm_state, QReject>::value);
    static_assert(std::is_same<tm_res, tm_true>::value);
    static_assert(std::is_same<tm_state, QAccept>::value);
    */
}