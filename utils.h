#ifndef ALGORITHMIQUE_UTILS_H
#define ALGORITHMIQUE_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

std::vector<std::string> splitString(std::string str, char delimiter);
std::vector<std::string> readWordListFromFile(std::string filename);
void writeResultsToFile(std::vector<bool> results, std::string filename);

#endif //ALGORITHMIQUE_UTILS_H
