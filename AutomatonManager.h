//
// Created by Sylvain on 05/04/2020.
//

#ifndef ALGORITHMIQUE_AUTOMATONMANAGER_H
#define ALGORITHMIQUE_AUTOMATONMANAGER_H

#include "Automaton.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class AutomatonManager {
public:
    static std::vector<std::string> readWordListFromFile(std::string filename);
    static void writeResultsToFile(std::vector<bool> results, std::string filename);
};


#endif //ALGORITHMIQUE_AUTOMATONMANAGER_H
