#include <string>
#include <unordered_set>

#include "util.h"

using namespace std;

const char valid_commands[] = {'>', '<', '+', '-', '.', '[', ']'};
const unordered_set<char> valid_command_set (std::begin(valid_commands), std::end(valid_commands));

int util::add(const int val, const int to_add, const int offset) {
    if (to_add < 0) {
        return util::subtract(val, -to_add, offset);
    }

    return (val + to_add) % offset;
}

int util::subtract(const int val, const int to_subtract, const int offset) {
    int diff = val - to_subtract;

    if (diff >= 0) return diff;

    return util::subtract(offset - 1, to_subtract - val - 1, offset);
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