//
// Created by Sylvain on 31/03/2020.
//

#include "Automaton.h"

Automaton::Automaton(std::string p_alphabet, std::vector<State> p_states, std::vector<Transition> p_transitions, std::vector<int> p_idxFinal, int p_idxInitial)
    : m_alphabet(p_alphabet), m_states(p_states), m_transitions(p_transitions), m_idxFinal(p_idxFinal), m_idxInitial(p_idxInitial)
{}

Automaton::Automaton(std::string filename)
    : m_alphabet(""), m_states(std::vector<State>()), m_transitions(std::vector<Transition>()), m_idxFinal(std::vector<int>()), m_idxInitial(-1)
{
    std::ifstream file(filename);
    std::string line;
    std::string alphabet;
    int nbStates;
    int nbTransitions;
    std::vector<std::string> transitionPerState;

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
            m_states.push_back(State(line[0] == '1', line[2] == '1', std::to_string(i)));

            // Store initial and final states
            if (line[0] == '1') {
                m_idxInitial = i;
            }
            if (line[2] == '1') {
                m_idxFinal.push_back(i);
            }
            // Transitions
            getline(file, line);
            nbTransitions = std::stoi(line);
            transitionPerState.push_back("");
            for(int j = 0; j < nbTransitions; ++j) {
                getline(file, line);
                transitionPerState[i] += line + "\n";
            }
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
}

Automaton::~Automaton() {}

std::string& Automaton::getAlphabet() {
    return m_alphabet;
}

bool Automaton::isWordAccepted(std::string word) {
    std::vector<State*> currentStates;
    std::vector<State*> tempStates;
    currentStates.push_back(&m_states[m_idxInitial]);

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
        // Update current states for non-deterministic
        currentStates = tempStates;
        tempStates.clear();
    }

    bool accepted(false);
    for(int i = 0; i < currentStates.size(); ++i) {
        if (currentStates.at(i)->isFinal()) {
            accepted = true;
            break;
        }
    }

    return accepted;
}

void Automaton::saveToFile(std::string filename) {
    std::ofstream file(filename);
    int nbTransitions(0);
    std::string tmpStr;
    if (file.is_open()) {
        // Alphabet
        file << m_alphabet << "\n";
        // Number of states
        file << m_states.size() << "\n";
        // States
        for(std::vector<State>::iterator states_it = m_states.begin(); states_it < m_states.end(); ++states_it) {
            file << states_it->isInitial() << " " << states_it->isFinal() << "\n";
            // Get corresponding transitions
            for(std::vector<Transition>::iterator trans_it = m_transitions.begin(); trans_it != m_transitions.end(); ++trans_it) {
                if (trans_it->getStateFrom() == &(*states_it)) {
                    ++nbTransitions;
                    tmpStr += trans_it->getStateTo()->getName() + " " + trans_it->getLabel() + "\n";
                }
            }
            file << nbTransitions << "\n" << tmpStr;
            nbTransitions = 0;
            tmpStr = "";
        }
        file.close();
    } else {
        std::cerr << "File " << filename << " couldn't be opened.\n";
        exit(1);
    }
}

void Automaton::minimizeDeterministicAutomaton() {
    std::vector<std::vector<int>> matrix; // Contains the table of the Moore algorithm
    std::vector<int> column; // Contains one column being created
    std::vector<int> classColumn; // Contains the column containing the "states" (it's ~0, ~1 etc)
    bool fillClassColumn(true);

    // Fill with state label
    // Each char of alphabet -> write column -> write state label
    for(std::string::iterator alph_it = m_alphabet.begin(); alph_it != m_alphabet.end(); ++alph_it) {
        for(std::vector<State>::iterator state_it = m_states.begin(); state_it != m_states.end(); ++state_it) {
            // Get corresponding transition
            if (fillClassColumn) {
                classColumn.push_back(state_it->isFinal());
            }
            for(std::vector<Transition>::iterator trans_it = m_transitions.begin(); trans_it != m_transitions.end(); ++trans_it) {
                if (trans_it->getLabel() == std::string(1, *alph_it) && trans_it->getStateFrom() == &(*state_it)) {
                    column.push_back(std::stoi(trans_it->getStateTo()->getName()));
                    break;
                }
            }
        }
        fillClassColumn = false;
        matrix.push_back(column);
        column.clear();
    }
    matrix.push_back(classColumn);
    classColumn.clear();

    // First columns are created
    // Now we need to fill the other ones until the end
    int index(0);
    int classColumnIndex(m_alphabet.size());
    std::vector<std::string> newClasses; // Contains the newly created "classes" (~1 etc)
    std::vector<std::string> classes; // Contains the classes created just before

    //Initialize class string
    for(int i = 0; i < matrix.at(0).size(); ++i) {
        classes.push_back("");
    }
    for(int i = 0; i < matrix.size(); ++i) {
        for(int j = 0; j < matrix.at(i).size(); ++j) {
            classes.at(j) += std::to_string(matrix.at(i).at(j));
        }
    }

    do {
        classes = newClasses;
        newClasses.clear();
        for(int i = 0; i < matrix.at(0).size(); ++i) {
            newClasses.push_back(std::to_string(matrix.at(classColumnIndex).at(i)));
        }

        // Create new columns for each symbol of the alphabet
        for(int a = 0; a < m_alphabet.size(); ++a) {
            for(int i = 0; i < matrix.at(0).size(); ++i) {
                index = matrix.at(a).at(i);
                column.push_back(matrix.at(classColumnIndex).at(index));
                newClasses.at(i) += std::to_string(column.back());
            }
            matrix.push_back(column);
            column.clear();
        }

        // Create new state column ~n
        std::vector<std::string> temp; // Stores classes that are already treated
        std::vector<std::string>::iterator it;
        matrix.push_back(std::vector<int>());
        for(int i = 0; i < newClasses.size(); ++i) {
            it = std::find(temp.begin(), temp.end(), newClasses.at(i));
            if (it != temp.end()) {
                //Class already treated
                matrix.at(matrix.size() - 1).push_back(std::distance(temp.begin(), it));
            } else {
                temp.push_back(newClasses.at(i));
                matrix.at(matrix.size() - 1).push_back(temp.size() - 1);
            }
        }

        //Cleanup
        temp.clear();
        classColumnIndex = matrix.size() - 1;
    } while(newClasses != classes);

    // The matrix is now completed
    // The states and transitions will be created, and the automaton updated

    int matrixLastColumn = matrix.size() - 1;
    // Get new states names
    std::vector<int> states;
    std::vector<int>::iterator it;
    // Remove duplicates
    for(int i = 0; i < matrix.at(matrixLastColumn).size(); ++i) {
        it = std::find(states.begin(), states.end(), matrix.at(matrixLastColumn).at(i));
        if (it == states.end()) {
            //State is not yet in array
            states.push_back(matrix.at(matrixLastColumn).at(i));
        }
    }

    // Get class of final and initial states
    std::vector<int> finalStates;
    for(int i = 0; i < m_idxFinal.size(); ++i) {
        it = std::find(finalStates.begin(), finalStates.end(), matrix.at(matrixLastColumn).at(m_idxFinal.at(i)));
        if (it == finalStates.end()) {
            finalStates.push_back(matrix.at(matrixLastColumn).at(m_idxFinal.at(i)));
        }
    }
    int initialState(matrix.at(matrixLastColumn).at(m_idxInitial));

    //Create states objects
    m_states.clear();
    m_transitions.clear();
    bool final(false);
    bool initial(false);
    for(int i = 0; i < states.size(); ++i) {
        final = false;
        initial = false;
        it = std::find(finalStates.begin(), finalStates.end(), states.at(i));
        if (it != finalStates.end()) {
            final = true;
        }
        initial = states.at(i) == initialState;
        m_states.push_back(State(initial, final, std::to_string(states.at(i))));
    }

    // Create transitions
    for(int i = 0; i < matrix.at(matrix.size() - 1).size(); ++i) {
        if (std::find(states.begin(), states.end(), matrix.at(matrix.size() - 1).at(i)) != states.end()) {
            // Transitions of the state are not yet created
            for(int a = 0; a < m_alphabet.size(); ++a) {
                m_transitions.push_back(Transition(
                        std::string(1, m_alphabet.at(a)),
                        &m_states.at(matrix.at(matrixLastColumn).at(i)),
                        &m_states.at(matrix.at(matrixLastColumn - m_alphabet.size() + a).at(i)))
                        );
                // Mark state as already treated
                states.at(i) = -1;
            }
        }
    }
    m_idxFinal = finalStates;
    m_idxInitial = initialState;
}

void Automaton::makeDeterministic() {
    std::vector<std::string> statesQueue;
    statesQueue.push_back(m_states.at(m_idxInitial).getName() + ","); // State format is the number followed by a comma e.g. 0,1,
    std::string currentState;
    std::string destinationState;
    std::vector<std::string> alreadyTreated; // States that were already done
    std::vector<std::string>::iterator it;
    std::vector<std::vector<std::string>> matrix; // One column per state with one line per character of the alphabet. Contains the destination state

    while(!statesQueue.empty()) {
        // Get next state
        currentState = statesQueue.front();
        statesQueue.erase(statesQueue.begin());
        destinationState = "";
        alreadyTreated.push_back(currentState);
        matrix.push_back(std::vector<std::string>());

        std::vector<std::string> states = splitString(currentState, ',');
        // For each letter
        for(int a = 0; a < m_alphabet.size(); ++a) {
            // For each state in the string
            for(int i = 0; i < states.size(); ++i) {
                // Get transition
                for(std::vector<Transition>::iterator tra_it = m_transitions.begin(); tra_it != m_transitions.end(); ++tra_it) {
                    if (tra_it->getStateFrom() == &m_states.at(std::stoi(states.at(i))) && tra_it->getLabel() == std::string( 1,m_alphabet.at(a))) {
                        // Matching transition
                        destinationState += tra_it->getStateTo()->getName() + ",";
                    }
                }
            }

            // Store corresponding destination state
            it = std::find(alreadyTreated.begin(), alreadyTreated.end(), destinationState);
            if (it == alreadyTreated.end() && destinationState != "") {
                statesQueue.push_back(destinationState);
            }
            matrix.at(matrix.size() - 1).push_back(destinationState);
            destinationState = "";
        }
    }

    //Handle initial & final
    std::vector<bool> finals;
    std::vector<bool> initials;
    for (int i = 0; i < alreadyTreated.size(); ++i) {
        // Initial
        if (alreadyTreated.at(i) == std::to_string(m_idxInitial) + ",") {
            initials.push_back(true);
            m_idxInitial = i;
        } else {
            initials.push_back(false);
        }

        // Final
        finals.push_back(false);
        for (int j = 0; j < m_idxFinal.size(); ++j) {
            if (alreadyTreated.at(i).find(std::to_string(m_idxFinal.at(j))) != std::string::npos) {
                finals.at(finals.size() - 1) = true;
                break;
            }
        }
    }

    // Create states
    m_states = std::vector<State>();
    m_transitions = std::vector<Transition>();
    for (int i = 0; i < alreadyTreated.size(); ++i) {
        m_states.push_back(State(initials.at(i), finals.at(i), std::to_string(i)));
    }

    // Create transitions
    // For each state
    for (int i = 0; i < matrix.size(); ++i) {
        // For each letter of alphabet
        for (int j = 0; j < matrix.at(i).size(); ++j) {
            it = std::find(alreadyTreated.begin(), alreadyTreated.end(), matrix.at(i).at(j));
            m_transitions.push_back(Transition(std::string(1, m_alphabet.at(j)), &m_states.at(i),
                                               &m_states.at(std::distance(alreadyTreated.begin(), it))));
        }
    }
}