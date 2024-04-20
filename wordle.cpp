#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
//#include <algorithm> 
#include <map>
#include <set>
#include <vector>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

#include <unordered_map>

// Add prototypes of helper functions here
void generateWord(int idx, string& current, const string& in, const unordered_map<char, int>& floatingCounts, vector<bool>& used, const set<string>& dict, set<string>& results);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Preprocess floating letters into a map for efficient lookup
    unordered_map<char, int> floatingCounts;
    for (char c : floating) {
        if (floatingCounts.find(c) == floatingCounts.end()) {
            floatingCounts[c] = 1;
        } else {
            floatingCounts[c]++;
        }
    }

    // Initialize variables and call the word generation function
    set<string> results;
    string current = in;
    vector<bool> used(floating.size(), false);

    generateWord(0, current, in, floatingCounts, used, dict, results);
    return results;
}

// Define any helper functions here
void generateWord(int idx, string& current, const string& in, const unordered_map<char, int>& floatingCounts, vector<bool>& used, const set<string>& dict, set<string>& results) {
    if (idx == current.size()) {
        bool allUsed = true;
        for (bool val : used) {
            if (!val) {
                allUsed = false;
                break;
            }
        }
        if (allUsed && dict.find(current) != dict.end()) {
            results.insert(current);
        }
        return;
    }

    if (current[idx] != '-') { 
        generateWord(idx + 1, current, in, floatingCounts, used, dict, results);
    } 
    else {
        for (size_t i = 0; i < floatingCounts.size(); ++i) {
            char c = floatingCounts[i].first;
            int countInCurrent = count(current.begin(), current.end(), c);
            if (!used[i] && floatingCounts[c] > countInCurrent) {
                current[idx] = c;
                used[i] = true;
                generateWord(idx + 1, current, in, floatingCounts, used, dict, results);
                used[i] = false; 
            }
        }

        current[idx] = '-';
    }
}
