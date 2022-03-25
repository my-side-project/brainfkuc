#ifndef COMPILER_COMPILER_
#define COMPILER_COMPILER_

#include <vector>
#include <string>

#include "compiler_data.h"

using namespace std;

namespace compiler {
    const string CMD_PRINT = "PRINT";
    const string CMD_ADD = "ADD";
    const string CMD_MUL = "MUL";
    const string CMD_DIV = "DIV";
    const string CMD_COPY = "COPY";
    const string CMD_MOVE = "MOVE";
    const string CMD_JZ = "JZ";
    const string CMD_JNZ = "JNZ";

    vector<compiler_data::Node> compile(const string &program_text);
    void print_assembly(vector<compiler_data::Node> compiled);
}

#endif
