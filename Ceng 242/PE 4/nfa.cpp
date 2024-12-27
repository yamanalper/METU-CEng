#include "nfa.hpp"

// private:
bool NFA::has_state(const std::string& state) const{
    if(all_states.count(state) == 1) return true;
    return false;
}

bool NFA::is_final_state(const std::string& state) const{
    if(final_states.count(state) == 1) return true;
    return false;
}

// public:
bool NFA::is_DFA() const{
    for (const auto& state : all_states) {
            std::set<char> symbols_seen;
            for (const auto& symbol : alphabet.get_symbols()) {
                std::set<std::string> next_states = transitions(state, symbol);
                if (next_states.size() > 1) {
                    return false;
                }
            }
            std::set<std::string> empty_state = transitions(state, 'e');
            if(!empty_state.empty()) return false;
            
        }
        return true;
}

void NFA::update_state_name(const std::string& old_name, const std::string& new_name){
    if (all_states.find(old_name) != all_states.end()) {
        all_states.erase(old_name);
        all_states.insert(new_name);
    }
    if (starting_state == old_name) starting_state = new_name;
    if (final_states.find(old_name) != final_states.end()) {
        final_states.erase(old_name);
        final_states.insert(new_name);
    }
    
}

bool NFA::process(std::string input) {
    if (!alphabet.is_valid(input)) {
        std::cout << "Invalid string\n";
        return false;
    }

    std::queue<ComputationBranch> queue;
    queue.push(ComputationBranch(starting_state, input));

    while (!queue.empty()) {
        ComputationBranch branch = queue.front();
        queue.pop();

        std::pair<std::string, std::string> config = branch.get_last_config();
        std::string currentState = config.first;
        std::string remainingInput = config.second;
        
        if (is_final_state(currentState) && remainingInput.empty()) {
            std::cout << branch << std::endl;
            std::cout << "Accept\n";
            return true;
        }

        if (!remainingInput.empty()) {
            char currentSymbol = remainingInput.front();
            std::string nextInput = remainingInput.substr(1);

            std::set<std::string> nextStates = transitions(currentState, currentSymbol);
            for (const std::string& nextState : nextStates) {
                ComputationBranch nextBranch = branch; 
                nextBranch.push_config(nextState, nextInput);
                queue.push(nextBranch);
            }
        }
        if(queue.empty()){
            std::cout << branch << std::endl;
            std::cout << "Reject\n";
            return false;
        }
    }


    std::cout << "Reject\n";
    return false;
}

    
NFA NFA::operator!() const{
    if (!this->is_DFA()){
        std::cout << "Not a DFA" << std::endl;
    }
    NFA complementNFA;
    complementNFA.alphabet = alphabet;
    complementNFA.all_states = all_states;
    complementNFA.starting_state = starting_state;
    complementNFA.transitions = transitions;
    for (const auto& state : all_states) {
        if (final_states.count(state) == 0) {
            complementNFA.final_states.insert(state);
        }
    }

    return complementNFA;
}

NFA NFA::operator+(const NFA& other) const {
    NFA unionNFA;
    return unionNFA;
}