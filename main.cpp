#include <iostream>

#include "Automaton.h"

const int MODE_PARAM = 1;
const int AUTOMATON_FILE_PARAM = 2;

const std::vector<int> ACCEPTED_MODES{0, 1, 2};

int main(int argc, char *argv[]) {
    //Parse arguments
    if (argc < 4) {
        std::cerr << "The program expects at least 3 arguments, only " << --argc << " were given.\n";
        std::cerr << "Usage: lanceur <mode> <automaton_file> <words_file> <out_file>\n";
        std::cerr << "With:\n";
        std::cerr << "\tmode\t\t0 to test the automaton against words\n";
        std::cerr << "\t\t\t1 to minimize the automaton\n";
        std::cerr << "\t\t\t2 to make the automaton deterministic\n";
        std::cerr << "\tautomaton_file\ta text file containing the description of the automaton\n";
        std::cerr
                << "\twords_file\ta text file containing the words to test against the automaton (only needed for mode 0)\n";
        std::cerr << "\tout_file\tfile to write the output\n";
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
                std::cout << "Performing minimization...\n";
                automaton.minimizeDeterministicAutomaton();
                std::cout << "Writing new automaton to " << argv[3] << "...\n";
                automaton.saveToFile(argv[3]);
                break;
            }
            case 2: {
                // Make deterministic
                std::cout << "MAKE DETERMINISTIC\n";
                std::cout << "Performing determination...\n";
                automaton.makeDeterministic();
                std::cout << "Writing new automaton to " << argv[3] << "...\n";
                automaton.saveToFile(argv[3]);
                break;
            }
        }
    } else {
        std::cerr << "The given mode parameter is invalid.\n";
        exit(1);
    }

    std::cout << "Done.";
}
