#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>

#include "utilities.hpp"

std::string ProfileDNA(const std::string& dna_database, const std::string& dna_sequence);

int FindLongestRun(const std::string& dna_sequence, const std::string& dna_strand);

std::vector<std::vector<std::string>> ReadDNA(const std::string& dna_database);

#endif