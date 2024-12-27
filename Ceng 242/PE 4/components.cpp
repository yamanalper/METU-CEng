#include "components.hpp"

/* operator<< overloads */
std::ostream& operator<<(std::ostream& os, Alphabet& a){
    for (auto it = a.symbols.begin(); it != a.symbols.end(); ++it) {
        os << *it;
        if (std::next(it) != a.symbols.end()) {
            os << ' ';
        }
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, const Rule& r){
    os << r.initial_state << " " << r.symbol << " " << r.final_state;
    return os;
}
std::ostream& operator<<(std::ostream& os, TransitionTable& t){
    for(int i = 0; i < t.rules.size(); i++){
        os << t.rules[i] << std::endl;
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, const ComputationBranch& c){
    for (int i = 0; i < c.config_history.size(); ++i) {
        const std::pair<std::string, std::string>& config = c.config_history[i];
        os << "(" << config.first << ", ";
        if (config.second.empty()) {
            os << "e)";
        } else {
            os << config.second << ")";
        }
        if (i < c.config_history.size() - 1) {
            os << " :- ";
        }
    }
    return os;
}


/* Alphabet */
bool Alphabet::is_valid(const std::string& input) const{
    bool check = false;
    for (int i = 0 ; i < input.length() ; i++ ){
        if ( symbols.count(input[i]) == 1 ) check = true;
        if (check == false) return false;
        check = false;
    }
    return true;
}

const std::set<char> Alphabet::get_symbols() const { 
    return symbols; 
}

Alphabet& Alphabet::operator+=(const Alphabet& other) {
    for (const char& symbol : other.get_symbols()) {
        this->symbols.insert(symbol);
    }
    return *this;
}


/* Rule */
Rule::Rule(const std::string& init_s, char symbol, const std::string& final_s){
    initial_state = init_s;
    this->symbol = symbol;
    final_state = final_s;
}

const std::string Rule::get_final_state() const {
    return final_state;
}

void Rule::update_state_name(const std::string& old_name, const std::string& new_name){
    if (old_name == final_state) final_state = new_name;
    if (old_name == initial_state) initial_state = new_name;
}

bool Rule::applies_to(const std::string& c, char s) const{
    if (c == initial_state && s == symbol) return true;
    return false;
}


/* TransitionTable */
void TransitionTable::add_rule(const std::string& initial_state, char symbol, const std::string& final_state){
    Rule newR = Rule(initial_state,symbol,final_state);
    rules.push_back(newR);
}

void TransitionTable::update_state_name(const std::string& old_name, const std::string& new_name){
    for(int i = 0 ; i<rules.size(); i++){
        rules[i].update_state_name(old_name, new_name);
    }
}

const std::set<std::string> TransitionTable::operator()(const std::string& curr_state, char symbol) const{
    std::set<std::string> next_states;
    for(int i = 0; i<rules.size(); i++){
        if ( rules[i].applies_to(curr_state,symbol) ) next_states.insert(rules[i].get_final_state());
    }
    return next_states;
}

TransitionTable& TransitionTable::operator+=(const TransitionTable& other){
    for (int i = 0; i<other.rules.size(); i++){
        this->rules.push_back(other.rules[i]);
    }
    return *this;
}


/* ComputationBranch */
void ComputationBranch::push_config(const std::string& state, const std::string& input){
    config_history.push_back(std::make_pair(state,input));
}

const std::pair<std::string,std::string> ComputationBranch::get_last_config() const{
    return config_history.back();
}