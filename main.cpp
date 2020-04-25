#include <iostream>

#include "Automaton.h"
#include "AutomatonManager.h"

const int MODE_PARAM = 1;
const int AUTOMATON_FILE_PARAM = 2;
const int WORDS_FILE_PARAM = 3;
const int OUT_FILE_PARAM = 4;

const std::vector<int> ACCEPTED_MODES {0, 1, 2};

int main(int argc, char* argv[]) {
    //Parse arguments
    if (argc < 5) {
        std::cerr << "The program expects 4 arguments, only " << --argc << " were given.\n";
        std::cerr << "Usage: lanceur <mode> <automaton_file> <words_file> <out_file>\n";
        std::cerr << "With:\n";
        std::cerr << "\tmode\t\t0 to test the automaton against words\n";
        std::cerr << "\t\t\t1 to minimize the automaton\n";
        std::cerr << "\t\t\t2 to make the automaton deterministic\n";
        std::cerr << "\tautomaton_file\ta text file containing the description of the automaton\n";
        std::cerr << "\twords_file\ta text file containing the words to test against the automaton\n";
        std::cerr << "\tout_file\tfile to write the output\n";
        return 1;
    }
    /*std::cout << "Mode: " << argv[MODE_PARAM] << "\n";
    std::cout << "Automaton file: " << argv[AUTOMATON_FILE_PARAM] << "\n";
    std::cout << "Words file: " << argv[WORDS_FILE_PARAM] << "\n";
    std::cout << "Output file: " << argv[OUT_FILE_PARAM] << "\n";*/

    int modeParam = std::atoi(argv[MODE_PARAM]);
    if(std::find(ACCEPTED_MODES.begin(), ACCEPTED_MODES.end(), modeParam) != ACCEPTED_MODES.end()) {
        // Mode is accepted
        // Create automaton from file
        Automaton a = Automaton(argv[AUTOMATON_FILE_PARAM]);

        switch(modeParam) {
            case 0: {
                // Test automaton
                std::cout << "TEST AUTOMATON\n";
                std::cout << "\tReading words from " << argv[WORDS_FILE_PARAM] << "...\n";
                std::vector<std::string> words = AutomatonManager::readWordListFromFile(argv[WORDS_FILE_PARAM]);
                std::vector<bool> results;
                std::cout << "\tTesting words...\n";
                for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
                    results.push_back(a.isWordAccepted(*it));
                }
                std::cout << "\tWriting results to " << argv[OUT_FILE_PARAM] << "...\n";
                AutomatonManager::writeResultsToFile(results, argv[OUT_FILE_PARAM]);
                break;
            }
            case 1: {
                // Minimize automaton
                std::cout << "MINIMIZE AUTOMATON\n";
                std::cout << "Performing minimization...\n";
                a.minimizeDeterministicAutomaton();
                std::cout << "Writing new automaton to " << argv[OUT_FILE_PARAM] << "...\n";
                a.saveToFile(argv[OUT_FILE_PARAM]);
                break;
            }
            case 2: {
                // Make deterministic
                std::cout << "MAKE DETERMINISTIC\n";
                std::cout << "Performing determination...\n";
                a.makeDeterministic();
                std::cout << "Writing new automaton to " << argv[OUT_FILE_PARAM] << "...\n";
                a.saveToFile(argv[OUT_FILE_PARAM]);
                break;
            }
        }
    } else {
        std::cerr << "The given mode parameter is invalid.\n";
        exit(1);
    }

    std::cout << "Done.";
}
