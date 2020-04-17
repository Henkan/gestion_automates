//
// Created by Sylvain on 05/04/2020.
//

#include "AutomatonManager.h"

std::vector<std::string> AutomatonManager::readWordListFromFile(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::string> words;
    std::string line;

    if (file.is_open()) {
        while(getline(file, line)) {
            words.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "File " << filename << " couldn't be opened.\n";
        exit(1);
    }
    return words;
}

void AutomatonManager::writeResultsToFile(std::vector<bool> results, std::string filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for(std::vector<bool>::iterator it = results.begin(); it != results.end(); ++it) {
            file << *it << "\n";
        }
        file.close();
    } else {
        std::cerr << "File " << filename << " couldn't be opened.\n";
        exit(1);
    }
}