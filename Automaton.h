#ifndef ALGORITHMIQUE_AUTOMATON_H
#define ALGORITHMIQUE_AUTOMATON_H

#include "State.h"
#include "Transition.h"
#include "utils.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

class Automaton {
private:
    std::string m_alphabet;
    std::vector<State> m_states;
    std::vector<Transition> m_transitions;
    std::vector<int> m_idxFinal;
    int m_idxInitial;
public:
    Automaton(std::string p_alphabet, std::vector<State> p_states, std::vector<Transition> p_transitions, std::vector<int> p_idxFinal, int p_idxInitial);
    Automaton(std::string filename);
    std::string& getAlphabet();
    bool isWordAccepted(std::string word);
    void saveToFile(std::string filename);
    void minimizeDeterministicAutomaton();
    void makeDeterministic();
};


#endif //ALGORITHMIQUE_AUTOMATON_H
