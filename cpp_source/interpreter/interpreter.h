#ifndef INTERPRETER_INTERPRETER_
#define INTERPRETER_INTERPRETER_

#include <vector>
#include "../compiler/compiler_data.h"

using namespace std;

namespace interpreter {
    // Returns total instructions interpreted.
    int interpret(const vector<compiler_data::Node> &compiled_program, const bool silent);
}

#endif