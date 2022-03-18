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

vector<Node> compiler::compile(const string &program_text) {
    string program = util::clean_program(program_text);

    stack<int> stack;
    vector<Node> compiled_program;

    char last_instr = '\0';
    int last_instr_qnt = 0;

    int line = 0;

    string::iterator it;
    for (it = program.begin(); it != program.end(); it++) {
        char command = *it;

        // If we finished an instruction stream, flush to output
        if (last_instr != '\0' && last_instr != command) {
            compiled_program.push_back(Node (line++, last_instr, last_instr_qnt, -1));

            last_instr = '\0';
            last_instr_qnt = 0;
        }

        if (command == '[') {
            stack.push(line);
            compiled_program.push_back(Node (line++, command, 1, -1));
        } else if (command == ']') {
            compiled_program[stack.top()].set_jump_to(line);
            int jump_line = compiled_program[stack.top()].get_line();
            stack.pop();

            compiled_program.push_back(Node (line++, command, 1, jump_line));
        } else if (stackable_commands_set.find(command) != stackable_commands_set.end()) {
            if (last_instr == '\0') last_instr = command;
            last_instr_qnt += 1;
        } else {
            // This is just the "." command that we'd just push without any magic.
            compiled_program.push_back(Node (line++, command, 1, -1));
        }
    }

    if (last_instr != 0) {
        compiled_program.push_back(Node (line++, last_instr, last_instr_qnt, -1));
    }

    return compiled_program;
}

void compiler::print_assembly(vector<Node> compiled) {
    vector<Node>::iterator it;

    for (it = compiled.begin(); it != compiled.end(); it++) {
        Node node = *it;

        switch (node.get_command())
        {
            case '+':
                cout << node.get_line() << ": ADD " << node.get_qnt() << endl;
                break;
            case '-':
                cout << node.get_line() << ": SUB " << node.get_qnt() << endl;
                break;
            case '>':
                cout << node.get_line() << ": RIGHT " << node.get_qnt() << endl;
                break;
            case '<':
                cout << node.get_line() << ": LEFT " << node.get_qnt() << endl;
                break;
            case '.':
                cout << node.get_line() << ": PRINT" << endl;
                break;
            case '[':
                cout << node.get_line() << ": JZ " << node.get_jump_to() << endl;
                break;
            case ']':
                cout << node.get_line() << ": JNZ " << node.get_jump_to() << endl;
                break;
        }
    }
}

compiler::Node::Node(int line, char command, int qnt, int jump_to) {
    this->line = line;
    this->command = command;
    this->qnt = qnt;
    this->jump_to = jump_to;
}

void compiler::Node::set_jump_to(int jump_to) {
    this->jump_to = jump_to;
}

char compiler::Node::get_command() {
    return this->command;
}

int compiler::Node::get_jump_to() {
    return this->jump_to;
}

int compiler::Node::get_qnt() {
    return this->qnt;
}

int compiler::Node::get_line() {
    return this->line;
}
