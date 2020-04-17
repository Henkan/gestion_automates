#ifndef ALGORITHMIQUE_AUTOMATON_H
#define ALGORITHMIQUE_AUTOMATON_H

#include "State.h"
#include "Transition.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "utils.h"

class Automaton {
private:
    std::string m_alphabet;
    std::vector<State> m_states;
    std::vector<Transition> m_transitions;
    int m_idxFinal;
    int m_idxInitial; //TODO: maybe vector
public:
    Automaton(std::string p_alphabet, std::vector<State> p_states, std::vector<Transition> p_transitions, int p_idxFinal, int p_idxInitial);
    Automaton(std::string filename);
    virtual ~Automaton();
    std::string& getAlphabet();
    bool isWordAccepted(std::string word);
};


#endif //ALGORITHMIQUE_AUTOMATON_H
