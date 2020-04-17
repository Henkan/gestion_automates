#include <iostream>

#include "Automaton.h"
#include "AutomatonManager.h"

const int MODE_PARAM = 1;
const int AUTOMATON_FILE_PARAM = 2;
const int WORDS_FILE_PARAM = 3;
const int OUT_FILE_PARAM = 4;

int main(int argc, char* argv[]) {
    //Parse arguments
    if (argc < 5) {
        std::cerr << "The program expects 4 arguments, only " << --argc << " were given.\n";
        std::cerr << "Usage: lanceur <mode> <automaton_file> <words_file> <out_file>\n";
        std::cerr << "With:\n";
        std::cerr << "\tmode\t\t0 to test the automaton\n";
        std::cerr << "\t\t\t1 to minimize the automaton\n";
        std::cerr << "\tautomaton_file\ta text file containing the description of the automaton\n";
        std::cerr << "\twords_file\ta text file containing the words to test against the automaton\n";
        std::cerr << "\tout_file\tfile to write the output\n";
        return 1;
    }
    std::cout << "Mode: " << argv[MODE_PARAM] << "\n";
    std::cout << "Automaton file: " << argv[AUTOMATON_FILE_PARAM] << "\n";
    std::cout << "Words file: " << argv[WORDS_FILE_PARAM] << "\n";
    std::cout << "Output file: " << argv[OUT_FILE_PARAM] << "\n";

    // Create automaton from file
    Automaton a = Automaton(argv[AUTOMATON_FILE_PARAM]);
    std::vector<std::string> words = AutomatonManager::readWordListFromFile(argv[WORDS_FILE_PARAM]);
    std::vector<bool> results;
    for(std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
        results.push_back(a.isWordAccepted(*it));
    }

    AutomatonManager::writeResultsToFile(results, argv[OUT_FILE_PARAM]);
    //TODO: Automaton -> set instead of vector ? (with name)
}
