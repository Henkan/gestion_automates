#include "utils.h"

std::vector<std::string> splitString(std::string str, char delimiter) {
    std::vector<std::string> rep;
    std::string word("");
    for (std::string::iterator str_it = str.begin(); str_it != str.end(); ++str_it) {
        if (*str_it != delimiter) {
            word += *str_it;
        } else if (word != "") {
            rep.push_back(word);
            word = "";
        }
    }
    if (word != "") rep.push_back(word);
    return rep;
}

std::vector<std::string> readWordListFromFile(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::string> words;
    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            words.push_back(line);
        }
        file.close();
    } else {
        std::cerr << "File " << filename << " couldn't be opened.\n";
        exit(1);
    }
    return words;
}

void writeResultsToFile(std::vector<bool> results, std::string filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (std::vector<bool>::iterator it = results.begin(); it != results.end(); ++it) {
            file << *it << "\n";
        }
        file.close();
    } else {
        std::cerr << "File " << filename << " couldn't be opened.\n";
        exit(1);
    }
}