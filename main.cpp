#include <iostream>

#include "Automaton.h"

const int MODE_PARAM = 1;
const int AUTOMATON_FILE_PARAM = 2;

const std::vector<int> ACCEPTED_MODES{0, 1, 2, 3};

int main(int argc, char *argv[]) {
    //Parse arguments
    if (argc < 4) {
        // Not enough args, print usage
        std::cerr << "The program expects at least 3 arguments, only " << --argc << " were given.\n";
        std::cout << "Usage: lanceur <mode> <automaton_file> <words_file> <out_file>\n";
        std::cout << "With:\n";
        std::cout << "\tmode\t\t0 to test the automaton against words\n";
        std::cout << "\t\t\t1 to minimize the automaton\n";
        std::cout << "\t\t\t2 to make the automaton deterministic\n";
        std::cout << "\t\t\t3 to remove epsilon transitions\n";
        std::cout << "\tautomaton_file\ta text file containing the description of the automaton\n";
        std::cout
                << "\twords_file\ta text file containing the words to test against the automaton (only needed for mode 0)\n";
        std::cout << "\tout_file\tfile to write the output\n";
        std::cout << "\tAny additional parameter or file will not be used.\n";
        return 1;
    }

    int modeParam = std::atoi(argv[MODE_PARAM]);
    if (std::find(ACCEPTED_MODES.begin(), ACCEPTED_MODES.end(), modeParam) != ACCEPTED_MODES.end()) {
        // Mode is accepted
        // Create automaton from file
        Automaton automaton = Automaton(argv[AUTOMATON_FILE_PARAM]);

        switch (modeParam) {
            case 0: {
                // Test automaton
                if (argc < 5) {
                    std::cerr << "Not enough arguments for this mode.\n";
                    exit(1);
                }
                std::cout << "TEST AUTOMATON\n";
                std::cout << "\tReading words from " << argv[3] << "...\n";
                std::vector<std::string> words = readWordListFromFile(argv[3]);
                std::vector<bool> results;
                std::cout << "\tTesting words...\n";
                for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
                    results.push_back(automaton.isWordAccepted(*it));
                }
                std::cout << "\tWriting results to " << argv[4] << "...\n";
                writeResultsToFile(results, argv[4]);
                break;
            }
            case 1: {
                // Minimize automaton
                std::cout << "MINIMIZE AUTOMATON\n";
                std::cout << "\tPerforming minimization...\n";
                automaton.minimizeDeterministicAutomaton();
                std::cout << "\tWriting new automaton to " << argv[3] << "...\n";
                automaton.saveToFile(argv[3]);
                break;
            }
            case 2: {
                // Make deterministic
                std::cout << "MAKE DETERMINISTIC\n";
                std::cout << "\tPerforming determination...\n";
                automaton.makeDeterministic();
                std::cout << "\tWriting new automaton to " << argv[3] << "...\n";
                automaton.saveToFile(argv[3]);
                break;
            }
            case 3: {
                // Remove epsilon transitions
                std::cout << "HANDLE EPSILON TRANSITIONS\n";
                std::cout << "\tMerging equivalent states from epsilon-transitions...\n";
                automaton.mergeEquivalentStates();
                std::cout << "\tRemoving remaining epsilon transitions...\n";
                automaton.removeEpsilonTransitions();
                std::cout << "\tWriting new automaton to " << argv[3] << "...\n";
                automaton.saveToFile(argv[3]);
                break;
            }
        }
    } else {
        std::cerr << "The given mode parameter is invalid.\n";
        exit(1);
    }

    std::cout << "Done.\n";
    std::cout << "Have a nice day\n";
}
