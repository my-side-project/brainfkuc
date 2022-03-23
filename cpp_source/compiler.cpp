#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_set>

#include "loop_compiler.hpp"
#include "compiler.hpp"
#include "util.hpp"
#include "compiler_data.hpp"

using namespace std;
using namespace compiler;
using namespace loop_compiler;

const char stackable_commands[] = { '>', '<', '+', '-' };
const unordered_set<char> stackable_commands_set(begin(stackable_commands), end(stackable_commands));

/*
    Assembly Definition
    -------------------

    ADD    <MEM_LOCATION> <LITERAL>  -- Add literal to the given memory location.
    ADD    <MEM_LOCATION> <REG_NO>   -- Add contents of a register to a mem location.

    MUL    <REG_NO> <LITERAL>  -- Multiply register with a literal
    DIV    <REG_NO> <LITERAL> <FLOOR>  -- Divide register with a literal

    COPY    <MEM_LOCATION> <LITERAL>  -- Copy a literal to a memory location.
    COPY    <MEM_LOCATION> <REG_NO>   -- Copy a register to a memory location.

    MOVE    <LITERAL>  -- Move the data pointer by given literal.

    JZ    <MEM_LOCATION> <LITERAL>  -- Jump by literal if the memory location is 0.
    JNZ   <MEM_LOCATION> <LITERAL>  -- Jump by literal if the memory location is not 0.

    PRNT   <MEM_LOCATION>  -- Print the given memory location.

    ADD     <NUM_LITERAL>  <NONE>    -- Add literal to the data pointer location.
    MOVE    <NUM_LITERAL>  <NONE>    -- Moves the data pointer by given positions.
    JZ      <NUM_LITERAL>  <NONE>    -- If current data is 0, jump to the given location.
    JNZ     <NUM_LITERAL>  <NONE>    -- If current data is not 0, jump to the given location.
    PRNT    <NONE>         <NONE>    -- Print the ascii value of the current data.

    Note: all memory locations are relative pointers to the current location of the data pointer
    so negative values are allowed.
 */

vector<compiler_data::Node> parseCode(const string &raw_program) {
    string program = util::clean_program(raw_program);
    cout << "Total BF commands " << program.size() <<endl;
    
    stack<int> stack;
    vector<compiler_data::Node> compiled_program;

    int line = 0, code_ptr = 0;
    while (code_ptr < program.size()) {
        char command = program[code_ptr];

        if (command == '.') {
            compiled_program.push_back(compiler_data::Node (line++, 0, "PRNT", -1, -1, -1, -1));
        } else if (command == '>' || command == '<') {
            int moves = 0;
            while (code_ptr < program.size() && (program[code_ptr] == '>' || program[code_ptr] == '<')) {
                moves += (program[code_ptr++] == '>' ? 1 : -1);
            }

            compiled_program.push_back(compiler_data::Node (line++, 0, "MOVE", moves, -1, -1, -1));
            continue;
        } else if (command == '+' || command == '-') {
            int adds = 0;
            while (code_ptr < program.size() && (program[code_ptr] == '+' || program[code_ptr] == '-')) {
                adds += (program[code_ptr++] == '+' ? 1 : -1);
            }

            compiled_program.push_back(compiler_data::Node (line++, 0, "ADD", adds, -1, -1, -1));
            continue;
        } else if (command == '[') {
            stack.push(line);
            compiled_program.push_back(compiler_data::Node (line++, 0, "JZ", -1, -1, -1, -1));
        } else if (command == ']') {
            int popped = stack.top(); stack.pop();
            
            compiled_program[popped].set_op1(line);
            compiled_program.push_back(compiler_data::Node (line++, 0, "JNZ", popped, -1, -1, -1));
        }

        code_ptr++;
    }

    cout << "Total commands after initial assembly pass " << compiled_program.size() << endl;

    return compiled_program;
}

vector<compiler_data::Node> compiler::compile(const string &program_text) {
    vector<compiler_data::Node> source = parseCode(program_text);
    loop_compiler::compile_flat_loops(source);
    return source;
}

void compiler::print_assembly(vector<compiler_data::Node> compiled) {
    vector<compiler_data::Node>::iterator it;

    for (it = compiled.begin(); it != compiled.end(); it++) {
        compiler_data::Node node = *it;
        string command = node.get_command();

        if (command == "PRNT") {
            cout << node.get_line() << ": " << command << endl;
        } else if (command == "MOVE" || command == "ADD" || command == "JZ" || command == "JNZ") {
            cout << node.get_line() << ": " << command << " " << node.get_op1() << endl;
        }
    }
}