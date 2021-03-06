#include "Automaton.h"

Automaton::Automaton(const std::string &filename)
        : m_alphabet(""), m_states(std::vector<State>()), m_transitions(std::vector<Transition>()),
          m_idxFinal(std::vector<int>()), m_idxInitial(std::vector<int>()) {
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

        for (int i = 0; i < nbStates; ++i) {
            // State
            getline(file, line);
            m_states.push_back(State(line.at(0) == '1', line[2] == '1', std::to_string(i)));

            // Store initial and final states
            if (line.at(0) == '1') {
                m_idxInitial.push_back(i);
            }
            if (line.at(2) == '1') {
                m_idxFinal.push_back(i);
            }
            // Transitions
            getline(file, line);
            nbTransitions = std::stoi(line);
            transitionPerState.push_back("");
            for (int j = 0; j < nbTransitions; ++j) {
                getline(file, line);
                transitionPerState.at(i) += line + "\n";
            }
        }
        file.close();
    } else {
        std::cerr << "File " << filename << " couldn't be opened.\n";
        exit(1);
    }

    // Create transitions object from strings and link them to corresponding state
    std::string token;
    std::vector<std::string> elements;
    for (int i = 0; i < transitionPerState.size(); ++i) {
        std::vector<std::string> listTransitions = splitString(transitionPerState.at(i), '\n');
        for (int j = 0; j < listTransitions.size(); ++j) {
            elements = splitString(listTransitions.at(j), ' ');
            if (elements.at(1) != "#" || i != std::stoi(elements.at(0))) {
                // Don't add recursive epsilon transitions
                m_transitions.push_back(
                        Transition(elements.at(1), &m_states.at(i), &m_states.at(std::stoi(elements.at(0)))));
            }
        }
    }
}

bool Automaton::isWordAccepted(std::string word) {
    std::vector<State *> currentStates;
    std::vector<State *> tempStates;
    for (int i = 0; i < m_idxInitial.size(); ++i) {
        currentStates.push_back(&m_states.at(m_idxInitial.at(i)));
    }

    bool stop(false);
    // Test for each letter of the word
    for (std::string::iterator str_it = word.begin(); str_it != word.end(); ++str_it) {
        // Get transitions of state
        stop = true;
        for (int i = 0; i < m_transitions.size(); ++i) {
            //Each possible state
            if (m_transitions.at(i).getLabel() == std::string(1, *str_it)) {
                //Transition has correct label
                for (int j = 0; j < currentStates.size(); ++j) {
                    if (m_transitions.at(i).getStateFrom() == currentStates.at(j)) {
                        // There is a match
                        stop = false;
                        tempStates.push_back(m_transitions.at(i).getStateTo());
                    }
                }
            }
        }
        // Stop if no transition corresponds to the letter
        if (stop) return false;

        // Update current states
        currentStates = tempStates;
        tempStates.clear();
    }

    bool accepted(false);
    for (std::vector<State *>::iterator it = currentStates.begin(); it != currentStates.end(); ++it) {
        if ((*it)->isFinal()) {
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
        for (std::vector<State>::iterator states_it = m_states.begin(); states_it < m_states.end(); ++states_it) {
            file << states_it->isInitial() << " " << states_it->isFinal() << "\n";
            // Get corresponding transitions
            for (std::vector<Transition>::iterator trans_it = m_transitions.begin();
                 trans_it != m_transitions.end(); ++trans_it) {
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
    std::vector<int> classColumn; // Contains the column containing the "states" (~0, ~1 etc.)
    bool fillClassColumn(true);

    // Fill with state label
    // Each char of alphabet -> write column -> write state label
    for (std::string::iterator alph_it = m_alphabet.begin(); alph_it != m_alphabet.end(); ++alph_it) {
        for (std::vector<State>::iterator state_it = m_states.begin(); state_it != m_states.end(); ++state_it) {
            // Get corresponding transition
            if (fillClassColumn) {
                // Only fill the class column once
                classColumn.push_back(state_it->isFinal());
            }
            // Fill column
            for (std::vector<Transition>::iterator trans_it = m_transitions.begin();
                 trans_it != m_transitions.end(); ++trans_it) {
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

    // First columns are created (up to ~0)
    // Now we need to fill the other ones until the end of the algorithm
    int index(0);
    int classColumnIndex(m_alphabet.size());
    std::vector<std::string> newClasses; // Contains the newly created "classes" (~1 etc)
    std::vector<std::string> classes; // Contains the classes created just before

    // Initialize class string array
    for (int i = 0; i < matrix.at(0).size(); ++i) {
        classes.push_back("");
    }
    // Create classes string
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.at(i).size(); ++j) {
            classes.at(j) += std::to_string(matrix.at(i).at(j));
        }
    }

    do {
        classes = newClasses;
        newClasses.clear();
        for (int i = 0; i < matrix.at(0).size(); ++i) {
            newClasses.push_back(std::to_string(matrix.at(classColumnIndex).at(i)));
        }

        // Create new columns for each symbol of the alphabet
        for (int a = 0; a < m_alphabet.size(); ++a) {
            for (int i = 0; i < matrix.at(0).size(); ++i) {
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
        for (int i = 0; i < newClasses.size(); ++i) {
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
    } while (newClasses != classes);

    // The matrix is now completed
    // The states and transitions will be created, and the automaton updated

    int matrixLastColumn = matrix.size() - 1;
    // Get new states names
    std::vector<int> states;
    std::vector<int>::iterator it;
    // Remove duplicates
    for (int i = 0; i < matrix.at(matrixLastColumn).size(); ++i) {
        it = std::find(states.begin(), states.end(), matrix.at(matrixLastColumn).at(i));
        if (it == states.end()) {
            //State is not yet in array
            states.push_back(matrix.at(matrixLastColumn).at(i));
        }
    }

    // Get class of final and initial states
    std::vector<int> finalStates;
    std::vector<int> initialStates;
    for (int i = 0; i < m_idxFinal.size(); ++i) {
        it = std::find(finalStates.begin(), finalStates.end(), matrix.at(matrixLastColumn).at(m_idxFinal.at(i)));
        if (it == finalStates.end()) {
            finalStates.push_back(matrix.at(matrixLastColumn).at(m_idxFinal.at(i)));
        }
    }
    // Deterministic automaton so only one initial state
    int initialState(matrix.at(matrixLastColumn).at(m_idxInitial.at(0)));
    initialStates.push_back(initialState);

    //Create states objects
    m_states.clear();
    m_transitions.clear();
    bool final(false);
    bool initial(false);
    for (int i = 0; i < states.size(); ++i) {
        final = false;
        initial = false;
        it = std::find(finalStates.begin(), finalStates.end(), states.at(i));
        if (it != finalStates.end()) {
            final = true;
        }
        initial = states.at(i) == initialState;
        m_states.push_back(State(initial, final, std::to_string(states.at(i))));
    }

    // Create transitions objects
    for (int i = 0; i < matrix.at(matrix.size() - 1).size(); ++i) {
        if (std::find(states.begin(), states.end(), matrix.at(matrix.size() - 1).at(i)) != states.end()) {
            // Transitions of the state are not yet created
            for (int a = 0; a < m_alphabet.size(); ++a) {
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
    // Update automaton
    m_idxFinal = finalStates;
    m_idxInitial = initialStates;
}

void Automaton::makeDeterministic() {
    std::vector<std::string> statesQueue; // Queue of states
    std::string initial;
    // Create string of initial states
    for (int i = 0; i < m_idxInitial.size(); ++i) {
        initial += m_states.at(m_idxInitial.at(i)).getName() +
                   ","; // State format is the number followed by a comma e.g. 0,1,
    }
    statesQueue.push_back(initial);

    std::string currentState;
    std::string destinationState;
    std::vector<std::string> alreadyTreated; // States that were already done
    std::vector<std::string>::iterator it; // Just some iterator used for std::find later on
    std::vector<std::string>::iterator it2;
    std::vector<std::vector<std::string>> matrix; // One column per state with one line per character of the alphabet. Contains the destination state

    while (!statesQueue.empty()) {
        // Get next state
        currentState = statesQueue.front();
        statesQueue.erase(statesQueue.begin());
        destinationState = "";
        alreadyTreated.push_back(currentState);
        matrix.push_back(std::vector<std::string>());

        std::vector<std::string> states = splitString(currentState, ',');
        // For each letter
        for (int a = 0; a < m_alphabet.size(); ++a) {
            // For each state in the string
            for (int i = 0; i < states.size(); ++i) {
                // Get transition
                for (std::vector<Transition>::iterator tra_it = m_transitions.begin();
                     tra_it != m_transitions.end(); ++tra_it) {
                    if (tra_it->getStateFrom() == &m_states.at(std::stoi(states.at(i))) &&
                        tra_it->getLabel() == std::string(1, m_alphabet.at(a))) {
                        // Matching transition
                        if (destinationState.find(tra_it->getStateTo()->getName()) == std::string::npos) {
                            destinationState += tra_it->getStateTo()->getName() + ",";
                        }
                    }
                }
            }

            // Store corresponding destination state
            // Add to queue only if not present
            it = std::find(alreadyTreated.begin(), alreadyTreated.end(), destinationState);
            it2 = std::find(statesQueue.begin(), statesQueue.end(), destinationState);
            if (it == alreadyTreated.end() && destinationState != "" && it2 == statesQueue.end()) {
                statesQueue.push_back(destinationState);
            }
            matrix.at(matrix.size() - 1).push_back(destinationState);
            destinationState = "";
        }
    }

    //Handle initial & final
    std::vector<bool> finals;
    std::vector<bool> initials;
    std::vector<int> tmpIdxFinal;
    std::vector<int> tmpIdxInitial;
    for (int i = 0; i < alreadyTreated.size(); ++i) {
        // Initial
        if (alreadyTreated.at(i) == initial) {
            initials.push_back(true);
            tmpIdxInitial.push_back(i);
        } else {
            initials.push_back(false);
        }

        // Final
        finals.push_back(false);
        for (int j = 0; j < m_idxFinal.size(); ++j) {
            if (alreadyTreated.at(i).find(std::to_string(m_idxFinal.at(j))) != std::string::npos) {
                // State contains at least one final state of the initial automaton
                finals.at(finals.size() - 1) = true;
                tmpIdxFinal.push_back(i);
                break;
            }
        }
    }

    // Create states objects
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
            // Get new index of the destination state
            it = std::find(alreadyTreated.begin(), alreadyTreated.end(), matrix.at(i).at(j));
            m_transitions.push_back(Transition(std::string(1, m_alphabet.at(j)), &m_states.at(i),
                                               &m_states.at(std::distance(alreadyTreated.begin(), it))));
        }
    }

    // Update automaton
    m_idxInitial = tmpIdxInitial;
    m_idxFinal = tmpIdxFinal;
}

void Automaton::mergeEquivalentStates() {
    // Store states with epsilon transition to check if they make a cycle
    std::vector<State *> statesToTry;
    for (int i = 0; i < m_states.size(); ++i) {
        if (!getEpsilonTransitions(&m_states.at(i)).empty()) {
            statesToTry.push_back(&m_states.at(i));
        }
    }

    State *initialState = statesToTry.at(0);
    State *currentState = initialState;
    std::vector<State *> searchedStates;
    std::vector<State *> queue;
    queue.push_back(initialState);
    std::vector<Transition> transitionsOfState;
    std::vector<State *>::iterator it;

    int loop = 0;

    bool cycleFound;
    do {
        cycleFound = false;
        do {
            // Look for a cycle
            currentState = queue.front();
            queue.erase(queue.begin());
            transitionsOfState = getEpsilonTransitions(currentState);
            // If no epsilon transition, no cycle anyway
            if (!transitionsOfState.empty()) {
                for (int i = 0; i < transitionsOfState.size(); ++i) {
                    // No epsilon transition on state after this epsilon transition, so no cycle anyway on this state
                    if (!getEpsilonTransitions(transitionsOfState.at(i).getStateTo()).empty()) {
                        if (transitionsOfState.at(i).getStateTo() == initialState) {
                            // Cycle found
                            cycleFound = true;
                            break;
                        }
                        // Add state if never searched before and has epsilon transitions
                        if (std::find(searchedStates.begin(), searchedStates.end(),
                                      transitionsOfState.at(i).getStateTo()) == searchedStates.end()) {
                            searchedStates.push_back(transitionsOfState.at(i).getStateTo());
                            queue.push_back(transitionsOfState.at(i).getStateTo());
                        }
                    }
                }
            }
        } while (!cycleFound && !queue.empty());

        if (cycleFound) {
            State *fusionState = searchedStates.front();
            // Fusion initial state & first state of queue
            // Adapt corresponding transitions
            for (int i = 0; i < m_transitions.size(); ++i) {
                if (m_transitions.at(i).getStateTo() == initialState) {
                    // Transitions going to initialState now go to new fusion state
                    m_transitions.at(i).setStateTo(fusionState);
                }
                if (m_transitions.at(i).getStateFrom() == initialState) {
                    // Transitions from initial state now go from fusion state
                    m_transitions.at(i).setStateFrom(fusionState);
                }
                if (m_transitions.at(i).getLabel() == "#" &&
                    m_transitions.at(i).getStateTo() == m_transitions.at(i).getStateFrom()) {
                    // Useless recursive e-transition, needs to be removed
                    m_transitions.at(i).setStateTo(nullptr);
                    m_transitions.at(i).setStateFrom(nullptr);
                }
            }
            // Remove recursive e-transitions tagged with nullptr
            m_transitions.erase(std::remove_if(
                    m_transitions.begin(), m_transitions.end(),
                    [](Transition &tra) { return (tra.getStateFrom() == nullptr || tra.getStateTo() == nullptr); }
            ), m_transitions.end());

            // Set initial and final
            fusionState->setFinal(initialState->isFinal() | fusionState->isFinal());
            fusionState->setInitial(initialState->isInitial() | fusionState->isInitial());

            // Get index of initial file to delete
            int toDelete(-1);
            for (int i = 0; i < m_states.size(); ++i) {
                if (m_states.at(i).getName() == initialState->getName()) {
                    toDelete = i;
                    break;
                }
            }

            // Create strings of updated transitions in order to re-create them
            // except for initial state since he has no more transitions
            std::vector<std::string> transitionStrings;
            std::string transition;
            for (int i = 0; i < m_states.size(); ++i) {
                transition = "";
                if (m_states.at(i).getName() != initialState->getName()) {
                    for (int j = 0; j < m_transitions.size(); ++j) {
                        if (m_transitions.at(j).getStateFrom() == &m_states.at(i)) {
                            transition += (m_transitions.at(j).getStateTo()->getName() + " " +
                                           m_transitions.at(j).getLabel() +
                                           "\n");
                        }
                    }
                    transitionStrings.push_back(transition);
                }
            }

            // Delete initial state since it is now fusion
            m_states.erase(m_states.begin() + toDelete);

            // Update transitions vector
            // Since state was deleted, all index are changed in array so transition pointer points to wrong state
            m_transitions = std::vector<Transition>();
            std::vector<std::string> split;
            std::vector<std::string> lineSplit;
            for (int i = 0; i < transitionStrings.size(); ++i) {
                split = splitString(transitionStrings.at(i), '\n');
                for (int j = 0; j < split.size(); ++j) {
                    lineSplit = splitString(split.at(j), ' ');
                    if (std::stoi(lineSplit.at(0)) > toDelete) {
                        // Need to adjust index of state since the removal moves the objects to the left
                        m_transitions.push_back(
                                Transition(lineSplit.at(1), &m_states.at(i),
                                           &m_states.at(std::stoi(lineSplit.at(0)) - 1)));
                    } else {
                        m_transitions.push_back(
                                Transition(lineSplit.at(1), &m_states.at(i),
                                           &m_states.at(std::stoi(lineSplit.at(0)))));
                    }
                }
            }

            //Update states label to match new index
            for (int i = 0; i < m_states.size(); ++i) {
                if (m_states.at(i).getName() != std::to_string(i)) {
                    m_states.at(i).setName(std::to_string(i));
                }
            }

            //Update states to try
            statesToTry.clear();
            for (int i = 0; i < m_states.size(); ++i) {
                if (!getEpsilonTransitions(&m_states.at(i)).empty()) {
                    statesToTry.push_back(&m_states.at(i));
                }
            }
        } // end if cycleFound

        // Cleanup for possible next round
        if (!statesToTry.empty()) {
            initialState = statesToTry.front();
            statesToTry.erase(statesToTry.begin());
            currentState = initialState;
            searchedStates.clear();
            queue.clear();
            queue.push_back(currentState);
            transitionsOfState.clear();
        }
    } while (!statesToTry.empty());

    //Update initial & final
    m_idxInitial.clear();
    m_idxFinal.clear();
    for (int i = 0; i < m_states.size(); ++i) {
        if (m_states.at(i).isInitial()) {
            m_idxInitial.push_back(i);
        }
        if (m_states.at(i).isFinal()) {
            m_idxFinal.push_back(i);
        }
    }
}

void Automaton::removeEpsilonTransitions() {
    State *currentState;
    State *destinationState;
    std::vector<Transition> transitionsToAdd;
    std::vector<Transition> epsilonTransitions;
    std::vector<Transition>::iterator it;
    int toDelete(-1);

    // Get a state with epsilon transition
    for (int i = 0; i < m_states.size(); ++i) {
        epsilonTransitions = getEpsilonTransitions(&m_states.at(i));
        if (!epsilonTransitions.empty()) {
            currentState = &m_states.at(i);
            break;
        }
    }

    while (!epsilonTransitions.empty()) {

        for (int i = 0; i < epsilonTransitions.size(); ++i) {
            destinationState = epsilonTransitions.at(i).getStateTo();

            // Update final and initial
            destinationState->setInitial(destinationState->isInitial() | currentState->isInitial());
            currentState->setFinal(destinationState->isFinal() | currentState->isFinal());

            for (int j = 0; j < m_transitions.size(); ++j) {
                // Each transition going to current state are copied and go to destination state
                if (m_transitions.at(j).getStateTo() == currentState && m_transitions.at(j).getLabel() != "#") {

                    // Check if transition to add doesn't already exist
                    bool addTransition = true;
                    for (int t = 0; t < m_transitions.size(); ++t) {
                        if (m_transitions.at(t).getLabel() == m_transitions.at(j).getLabel() &&
                            m_transitions.at(t).getStateFrom() == m_transitions.at(j).getStateFrom() &&
                            m_transitions.at(t).getStateTo() == destinationState) {
                            addTransition = false;
                            break;
                        }
                    }

                    if (addTransition) {
                        m_transitions.push_back(
                                Transition(m_transitions.at(j).getLabel(), m_transitions.at(j).getStateFrom(),
                                           destinationState));
                    }
                }

                // The transition corresponds to the current epsilon transition so it will be deleted
                if (m_transitions.at(j).getStateTo() == epsilonTransitions.at(i).getStateTo() &&
                    m_transitions.at(j).getStateFrom() == epsilonTransitions.at(i).getStateFrom()
                    && m_transitions.at(j).getLabel() == "#") {
                    toDelete = j;
                }
            }
            // Remove corresponding epsilon transition
            m_transitions.erase(m_transitions.begin() + toDelete);
        }

        // Get new list of epsilon transitions and corresponding state
        toDelete = -1;
        epsilonTransitions.clear();
        for (int i = 0; i < m_states.size(); ++i) {
            epsilonTransitions = getEpsilonTransitions(&m_states.at(i));
            if (!epsilonTransitions.empty()) {
                currentState = &m_states.at(i);
                break;
            }
        }
    }
}

std::vector<Transition> Automaton::getEpsilonTransitions(State *state) {
    std::vector<Transition> epsilonTransitions;
    for (int i = 0; i < m_transitions.size(); ++i) {
        if (m_transitions.at(i).getStateFrom() == state && m_transitions.at(i).getLabel() == "#") {
            epsilonTransitions.push_back(m_transitions.at(i));
        }
    }
    return epsilonTransitions;
}