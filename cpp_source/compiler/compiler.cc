#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_set>

#include "loop_compiler.h"
#include "compiler.h"
#include "compiler_data.h"

#include "../util/util.h"

using namespace std;
using namespace compiler;
using namespace compiler_data;
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
    COPY    <REG_NO> <MEM_LOCATION>   -- Copy a register to a memory location.
    COPY    <REG_NO> <REG_NO>         -- Copy a register to another register.

    MOVE    <LITERAL>  -- Move the data pointer by given literal.

    JZ    <MEM_LOCATION> <LITERAL>  -- Jump by literal if the memory location is 0.
    JNZ   <MEM_LOCATION> <LITERAL>  -- Jump by literal if the memory location is not 0.

    PRNT   <MEM_LOCATION>  -- Print the given memory location.

    Note: all memory locations are relative pointers to the current location of the data pointer
    so negative values are allowed.
 */

vector<Node> parseCode(const string &raw_program) {
    string program = util::clean_program(raw_program);
    
    stack<int> stack;
    vector<Node> compiled_program;

    int line = 0, code_ptr = 0;
    while (code_ptr < program.size()) {
        char command = program[code_ptr];

        if (command == '.') {
            compiled_program.push_back(Node(0, CMD_PRINT));
        } else if (command == '>' || command == '<') {
            int moves = 0;
            while (code_ptr < program.size() && (program[code_ptr] == '>' || program[code_ptr] == '<')) {
                moves += (program[code_ptr++] == '>' ? 1 : -1);
            }

            compiled_program.push_back(Node(0, CMD_MOVE, moves));

            line++;
            continue;
        } else if (command == '+' || command == '-') {
            int adds = 0;
            while (code_ptr < program.size() && (program[code_ptr] == '+' || program[code_ptr] == '-')) {
                adds += (program[code_ptr++] == '+' ? 1 : -1);
            }

            line++;
            compiled_program.push_back(Node(0, CMD_ADD, 0, adds));
            continue;
        } else if (command == '[') {
            stack.push(line);
            compiled_program.push_back(Node(0, CMD_JZ, 0));
        } else if (command == ']') {
            int popped = stack.top(); stack.pop();
            
            compiled_program[popped].set_op2(line - popped);
            compiled_program.push_back(Node(0, CMD_JNZ, 0, popped - line));
        }

        line++;
        code_ptr++;
    }

    return compiled_program;
}

vector<compiler_data::Node> compiler::compile(const string &program_text) {
    vector<compiler_data::Node> source = parseCode(program_text);
    return loop_compiler::compile_flat_loops(source);
}

void compiler::print_assembly(vector<compiler_data::Node> &compiled) {
    vector<compiler_data::Node>::iterator it;

    for (it = compiled.begin(); it != compiled.end(); it++) {
        compiler_data::Node node = *it;
        string command = node.get_command();

        cout << command << ":" << node.get_overload() << " " << node.get_op1() << " " << node.get_op2() << " " << node.get_op3() << " " << node.get_op4() << endl;
    }
}