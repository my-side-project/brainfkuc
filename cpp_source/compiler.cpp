#include <vector>
#include <string>
#include <unordered_set>

#include "compiler.hpp"
#include "util.hpp"

using namespace std;

const char stackable_commands[] = { '>', '<', '+', '-' };
const unordered_set<char> stackable_commands_set(begin(stackable_commands), end(stackable_commands));

vector<compiler::Node> compiler::compile(const string &program_text) {
    string program = util::clean_program(program_text);

    vector<compiler::Node> compiled_program;
    vector<compiler::Node> stack;

    char last_instr = '\0';
    int last_instr_qnt = 0;

    int line = 0;

    string::iterator command;
    for (command = program.begin(); command != program.end(); command++) {
        if (stackable_commands_set.find(*command) == stackable_commands_set.end()
                && last_instr != '\0') {
            
            Node::Node new_node(line++, last_instr, last_instr_qnt);
            compiled_program.push_back(new_node);
        }

        switch (*command)
        {
            case '[':
                Node loop_start (line++, *command, 1);

                stack.push_back(loop_start);
                compiled_program.push_back(loop_start);
                break;
            
            

            default:
                break;
        }
    }
}

class compiler::Node {};

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
