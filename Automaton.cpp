//
// Created by Sylvain on 31/03/2020.
//

#include "Automaton.h"

Automaton::Automaton(std::string p_alphabet, std::vector<State> p_states, std::vector<Transition> p_transitions, int p_idxFinal, int p_idxInitial)
    : m_alphabet(p_alphabet), m_states(p_states), m_transitions(p_transitions), m_idxFinal(p_idxFinal), m_idxInitial(p_idxInitial)
{}

Automaton::Automaton(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    std::string alphabet;
    int nbStates;
    int nbTransitions;
    int stateNumber(0);
    m_states = std::vector<State>();
    m_transitions = std::vector<Transition>();
    std::vector<std::string> transitionPerState;
    int idxInitial(-1);
    int idxFinal(-1);

    if (file.is_open()) {
        //Alphabet is on first line
        getline(file, alphabet);
        m_alphabet = alphabet;
        //Number of states on second line
        getline(file, line);
        nbStates = std::stoi(line);

        for(int i = 0; i < nbStates; ++i) {
            // State
            getline(file, line);
            m_states.push_back(State(line[0] == '1', line[2] == '1', std::to_string(stateNumber)));

            // Store initial and final states
            if (line[0] == '1') {
                idxInitial = stateNumber;
            } else if (line[2] == '1') {
                idxFinal = stateNumber;
            }
            // Transitions
            getline(file, line);
            nbTransitions = std::stoi(line);
            transitionPerState.push_back("");
            for(int j = 0; j < nbTransitions; ++j) {
                getline(file, line);
                transitionPerState[stateNumber] += line + "\n";
            }
            // Remove '\n' at the end
            /* transitionPerState[stateNumber] = transitionPerState[stateNumber].substr(0, transitionPerState[stateNumber].size() - 1); */
            stateNumber++;
        }
        file.close();
    } else {
        std::cerr << "File " << filename << " couldn't be opened.\n";
        exit(1);
    }

    // Create transitions object
    std::string token;
    for(int i = 0; i < transitionPerState.size(); ++i) {
        std::vector<std::string> trans = splitString(transitionPerState[i], '\n');
        for(int j = 0; j < trans.size(); ++j) {
            m_transitions.push_back(Transition(std::string(1, trans[j][2]), &m_states[i], &m_states[atoi(&trans[j][0])]));
        }
    }
    m_idxFinal = idxFinal;
    m_idxInitial = idxInitial;
}

Automaton::~Automaton() {
}

std::string& Automaton::getAlphabet() {
    return m_alphabet;
}

bool Automaton::isWordAccepted(std::string word) {
    std::vector<State*> currentStates;
    std::vector<State*> tempStates;
    currentStates.push_back(&m_states[m_idxInitial]);
    //State* currentState = &m_states[m_idxInitial];

    bool stop(false);
    // Test for each letter of the word
    for(std::string::iterator str_it = word.begin(); str_it != word.end(); ++str_it) {
        //Transitions of state
        stop = true;
        for(int i = 0; i < m_transitions.size(); ++i) {
            //Each possible state
            if(m_transitions[i].getLabel() == std::string(1, *str_it)) {
                //Transition has correct label
                for (int j = 0; j < currentStates.size(); ++j) {
                    if (m_transitions[i].getStateFrom() == currentStates[j]) {
                        //There is a match
                        stop = false;
                        tempStates.push_back(m_transitions[i].getStateTo());
                    }
                }
            }
        }
        // Stop if no transition corresponds to the letter
        if (stop) return false;
        currentStates = tempStates;
        tempStates.clear();
    }
    std::cout << currentStates.size() << "\n";
    return std::find(currentStates.begin(), currentStates.end(), &m_states[m_idxFinal]) != currentStates.end();
    //return currentState == &m_states[m_idxFinal];
}