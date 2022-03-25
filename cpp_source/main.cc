#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "compiler/compiler.h"
#include "compiler/compiler_data.h"
#include "interpreter/interpreter.h"

using namespace std;
using namespace compiler;
using namespace interpreter;

string get_code(string file_path) {
    string code;
    ifstream file (file_path);

    string line;
    while (getline(file, line)) {
        code += line;
    }

    file.close();

    return code;
}

int main(int argc, char** argv) {
    string command = argv[1];

    string file_name = argv[2];
    string code = get_code(file_name);
    
    if (command == "run") {
        interpret(code);
    } else {
        vector<compiler_data::Node> compiled = compile(code);
        print_assembly(compiled);
    }
}
