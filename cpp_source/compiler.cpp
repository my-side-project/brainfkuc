#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_set>

#include "compiler.hpp"
#include "util.hpp"

using namespace std;

const char stackable_commands[] = { '>', '<', '+', '-' };
const unordered_set<char> stackable_commands_set(begin(stackable_commands), end(stackable_commands));

vector<compiler::Node> compiler::compile(const string &program_text) {
    string program = util::clean_program(program_text);

    vector<compiler::Node> compiled_program;
    stack<compiler::Node> stack;

    char last_instr = '\0';
    int last_instr_qnt = 0;

    int line = 0;

    string::iterator command;
    for (command = program.begin(); command != program.end(); command++) {
        if (stackable_commands_set.find(*command) == stackable_commands_set.end()
                && last_instr != '\0') {
            
            Node new_node(line++, last_instr, last_instr_qnt);
            compiled_program.push_back(new_node);

            last_instr = '\0';
            last_instr_qnt = 0;
        }

        if (*command == '[') {

            Node loop_start (line++, *command, 1);
            stack.push(loop_start);
            compiled_program.push_back(loop_start);

        } else if (*command == ']') {
            
            Node popped = stack.top();
            stack.pop();
            popped.set_jump_to(line);

            Node loop_end (line++, *command, 1);
            loop_end.set_jump_to(popped.get_line());

            compiled_program.push_back(loop_end);

        } else if (stackable_commands_set.find(*command) != stackable_commands_set.end()) {
            if (last_instr == '\0') {
                last_instr = *command;
            }

            if (last_instr != *command) {
                Node stackable (line++, last_instr, last_instr_qnt);
                compiled_program.push_back(stackable);

                last_instr = *command;
                last_instr_qnt = 1;
            } else {
                last_instr_qnt += 1;
            }
        } else {
            Node node (line++, *command, 1);
            compiled_program.push_back(node);
        }
    }

    if (last_instr != '\0') {
        Node new_node(line++, last_instr, last_instr_qnt);
        compiled_program.push_back(new_node);
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
                cout << node.get_line() << ": JZ" << node.get_jump_to() << endl;
                break;
            case ']':
                cout << node.get_line() << ": JNZ" << node.get_jump_to() << endl;
                break;
        }
    }
}

compiler::Node::Node(int line, char command, int qnt) {
    this->line = line;
    this->command = command;
    this->qnt = qnt;
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
