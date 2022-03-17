#include <string>

#include "util.hpp"
#include "compiler.hpp"
#include "interpreter.hpp"

using namespace std;
using namespace util;
using namespace compiler;

const int heap_size = 30000;

string interpreter::interpret(const string &program_txt) {
    vector<Node> compiled = compile(program_txt);

    int code_ptr = 0;
    int data_ptr = 0;

    int heap[heap_size];
    for (int i=0; i<heap_size; i++) {
        heap[i] = 0;
    }

    string output;

    while (code_ptr < compiled.size()) {
        Node command = compiled[code_ptr];

        switch (command.get_command())
        {
            case '+':
                heap[data_ptr] = add(heap[data_ptr], command.get_qnt());
                break;
            case '-':
                heap[data_ptr] = subtract(heap[data_ptr], command.get_qnt());
                break;
            case '>':
                data_ptr += command.get_qnt();
                break;
            case '<':
                data_ptr -= command.get_qnt();
                break;
            case '.':
                output.push_back((char) heap[data_ptr]);
                break;
            case '[':
                if (heap[data_ptr] == 0) code_ptr = command.get_jump_to();
                break;
            case ']':
                if (heap[data_ptr] != 0) code_ptr = command.get_jump_to();
                break;
        }

        code_ptr += 1;
    }

    return output;
}
