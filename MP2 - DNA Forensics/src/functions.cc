#include "functions.hpp"
#include "utilities.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::string TrimString(const std::string& str) {
    size_t start = 0;
    size_t end = str.length() - 1;
    while (start <= end && std::isspace(str[start]) != 0) {
        start++;
    }
    while (end >= start && std::isspace(str[end]) != 0) {
        end--;
    }
    return str.substr(start, end - start + 1);
}

std::vector<std::vector<std::string>> ReadDNA(const std::string& dna_database) {
    std::vector<std::vector<std::string>> dna_data;
    std::ifstream ifs{dna_database};
    for (std::string line; std::getline(ifs, line); line = "") {
        std::vector<std::string> temp_row = utilities::GetSubstrs(line, ',');
        for (auto& element : temp_row) {
            // std::cout << "without trimstring:|" << element << "|" << std::endl;
            element = TrimString(element);  
            // std::cout << "with trimstring:|" << element << "|" <<std::endl;
        }
        dna_data.push_back(temp_row);
    }
    return dna_data;
}

int FindLongestRun(const std::string& dna_sequence, const std::string& dna_strand) {
    int longest_run = 0;
    unsigned int strand_length = dna_strand.length();
    unsigned int sequence_length = dna_sequence.length();

    for (unsigned int i = 0; i <= sequence_length - strand_length; i++) {
        int current_run = 0;
        while (dna_sequence.substr(i, strand_length) == dna_strand) {
            current_run++;
            i += strand_length;  
        }
        if (current_run > longest_run) {
            longest_run = current_run;
        }
    }
    return longest_run;
}

std::string ProfileDNA(const std::string& dna_database_str, const std::string& dna_sequence_str) {
    std::vector<std::vector<std::string>> dna_data_vector = ReadDNA(dna_database_str);
    std::vector<std::string> str_patterns = dna_data_vector[0];
    std::string matched_person = "No match";
    int matches_found = 0;
    for (unsigned int i = 1; i < dna_data_vector.size(); i++) {
        std::string person_name = dna_data_vector[i][0];
        bool all_strs_match = true;
        std::cout << "name: " << person_name << std::endl;
        for (unsigned int j = 1; j < str_patterns.size(); j++) {
            std::string current_str = str_patterns[j];
            int expected_count = std::stoi(dna_data_vector[i][j]); 
            int actual_count = FindLongestRun(dna_sequence_str, current_str);
            std::cout << "expected_count: " << expected_count << std::endl;
            std::cout << "actual_count: " << actual_count << std::endl;  

            if (expected_count != actual_count) {
                all_strs_match = false;
                break;
            }
        }
        if (all_strs_match) {
            matches_found++;  
            matched_person = person_name;
            if (matches_found > 1) {
                return "No match";
            }
        }
    }
    if (matches_found == 1) {
        return matched_person;
    } else {
        return "No match";
    }
}