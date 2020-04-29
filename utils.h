#ifndef ALGORITHMIQUE_UTILS_H
#define ALGORITHMIQUE_UTILS_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

/**
 * Split a string on the specified delimiter and returns an array of substring.
 * @param str The string to split
 * @param delimiter The separator of each substring
 * @return An array containing each substring
 */
std::vector<std::string> splitString(std::string str, char delimiter);

/**
 * Read each word contained in a file.
 * @param filename The file with the words
 * @return An array of words
 */
std::vector<std::string> readWordListFromFile(std::string filename);

/**
 * Write the results to the specified file.
 * @param results An array of bool containing the results
 * @param filename The file that will be written
 */
void writeResultsToFile(std::vector<bool> results, std::string filename);

#endif //ALGORITHMIQUE_UTILS_H
