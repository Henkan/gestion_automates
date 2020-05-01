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
#include <set>

class Automaton {
private:
    std::string m_alphabet;
    std::vector<State> m_states;
    std::vector<Transition> m_transitions;
    std::vector<int> m_idxFinal;
    std::vector<int> m_idxInitial;
public:
    /**
     * Construct the automaton from a file.
     * @param filename The file which contains the description of the automaton.
     */
    Automaton(const std::string &filename);

    /**
     * Test a word against the automaton to see if it's accepted.
     * @param word the word to test
     * @return true if word is accepted, false otherwise
     */
    bool isWordAccepted(std::string word);

    /**
     * Write the automaton to the specified file.
     * @param filename The file to which the automaton will be written.
     */
    void saveToFile(std::string filename);

    /**
     * Minimize the automaton. Only works if the automaton is deterministic.
     */
    void minimizeDeterministicAutomaton();

    /**
     * Make the automaton deterministic.
     */
    void makeDeterministic();

    /**
     * Merge equivalent states based on cycles of epsilon transitions.
     */
    void mergeEquivalentStates();

    /**
     * Remove epsilon transitions.
     */
    void removeEpsilonTransitions();

    /**
     * Get all the epsilon transitions of a given state.
     * @param state A pointer to the state
     * @return The list of transitions
     */
    std::vector<Transition> getEpsilonTransitions(State *state);
};

#endif //ALGORITHMIQUE_AUTOMATON_H
