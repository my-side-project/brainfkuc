#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_set>

#include "compiler.hpp"
#include "util.hpp"

using namespace std;
using namespace compiler;

const char stackable_commands[] = { '>', '<', '+', '-' };
const unordered_set<char> stackable_commands_set(begin(stackable_commands), end(stackable_commands));

/*
    Assembly Definition
    -------------------

    ADD   <NUM_LITERAL>  <NONE>    -- Add literal to the data pointer location.
    MOVE  <NUM_LITERAL>  <NONE>    -- Moves the data pointer by given positions.
    JZ    <NUM_LITERAL>  <NONE>    -- If current data is 0, jump to the given location.
    JNZ   <NUM_LITERAL>  <NONE>    -- If current data is not 0, jump to the given location.
    PRNT  <NONE>         <NONE>    -- Print the ascii value of the current data.
 */

vector<Node> parseCode(const string &raw_program) {
    string program = util::clean_program(raw_program);
    cout << "Total BF commands " << program.size() <<endl;
    
    stack<int> stack;
    vector<Node> compiled_program;

    int line = 0, code_ptr = 0;
    while (code_ptr < program.size()) {
        char command = program[code_ptr];

        if (command == '.') {
            compiled_program.push_back(Node (line++, "PRNT", -1, -1));
        } else if (command == '>' || command == '<') {
            int moves = 0;
            while (code_ptr < program.size() && (program[code_ptr] == '>' || program[code_ptr] == '<')) {
                moves += (program[code_ptr++] == '>' ? 1 : -1);
            }

            compiled_program.push_back(Node (line++, "MOVE", moves, -1));
            continue;
        } else if (command == '+' || command == '-') {
            int adds = 0;
            while (code_ptr < program.size() && (program[code_ptr] == '+' || program[code_ptr] == '-')) {
                adds += (program[code_ptr++] == '+' ? 1 : -1);
            }

            compiled_program.push_back(Node (line++, "ADD", adds, -1));
            continue;
        } else if (command == '[') {
            stack.push(line);
            compiled_program.push_back(Node (line++, "JZ", -1, -1));
        } else if (command == ']') {
            int popped = stack.top(); stack.pop();
            
            compiled_program[popped].set_op1(line);
            compiled_program.push_back(Node (line++, "JNZ", popped, -1));
        }

        code_ptr++;
    }

    cout << "Total commands after initial assembly pass " << compiled_program.size() << endl;

    return compiled_program;
}

vector<Node> compiler::compile(const string &program_text) {
    return parseCode(program_text);
}

void compiler::print_assembly(vector<Node> compiled) {
    vector<Node>::iterator it;

    for (it = compiled.begin(); it != compiled.end(); it++) {
        Node node = *it;
        string command = node.get_command();

        if (command == "PRNT") {
            cout << node.get_line() << ": " << command << endl;
        } else if (command == "MOVE" || command == "ADD" || command == "JZ" || command == "JNZ") {
            cout << node.get_line() << ": " << command << " " << node.get_op1() << endl;
        }
    }
}

compiler::Node::Node(int line, string command, int op1, int op2) {
    this->line = line;
    this->command = command;
    this->op1 = op1;
    this->op2 = op2;
}

void compiler::Node::set_op1(int op1) {
    this->op1 = op1;
}

void compiler::Node::set_op2(int op2) {
    this->op2 = op2;
}

string compiler::Node::get_command() {
    return this->command;
}

int compiler::Node::get_line() {
    return this->line;
}

int compiler::Node::get_op1() {
    return this->op1;
}

int compiler::Node::get_op2() {
    return this->op2;
}
