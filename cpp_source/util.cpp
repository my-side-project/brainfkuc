#include <string>
#include <unordered_set>

#include "util.hpp"

using namespace std;

const char valid_commands[] = {'>', '<', '+', '-', '.', '[', ']'};
const unordered_set<char> valid_command_set (std::begin(valid_commands), std::end(valid_commands));

int util::add(const int val, const int to_add) {
    return (val + to_add) % 256;
}

int util::subtract(const int val, const int to_subtract) {
    int diff = val - to_subtract;

    if (diff >= 0) return diff;

    return util::subtract(255, to_subtract - val - 1);
}

string util::clean_program(const string &program_text) {
    string output;

    string::const_iterator it;
    for (it = program_text.begin(); it != program_text.end(); it++) {
        if (valid_command_set.find(*it) != valid_command_set.end()) {
            output.push_back(*it);
        }
    }

    return output;
}