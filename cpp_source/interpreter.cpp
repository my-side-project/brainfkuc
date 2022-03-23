#include <string>

#include "util.hpp"
#include "compiler.hpp"
#include "compiler_data.hpp"
#include "interpreter.hpp"

using namespace std;
using namespace util;
using namespace compiler;
using namespace compiler_data;

const int cell_size = 256;
const int heap_size = 30000;


string interpreter::interpret(const string &program_txt) {
    vector<compiler_data::Node> compiled = compile(program_txt);

    int code_ptr = 0;
    int data_ptr = 0;

    int heap[heap_size];
    for (int i=0; i<heap_size; i++) {
        heap[i] = 0;
    }

    string output;

    while (code_ptr < compiled.size()) {
        compiler_data::Node node = compiled[code_ptr];
        string command = node.get_command();

        if (data_ptr < 0) {
            return "";
        }

        if (command == "PRNT") {
            output.push_back((char) heap[data_ptr]);
        } else if (command == "ADD") {
            if (node.get_op1() >= 0) {
                heap[data_ptr] = add(heap[data_ptr], node.get_op1(), cell_size);
            } else {
                heap[data_ptr] = subtract(heap[data_ptr], -node.get_op1(), cell_size);
            }
        } else if (command == "ADDM") {
            if (node.get_op1() >= 0) {
                heap[data_ptr + node.get_op1()] = add(heap[data_ptr + node.get_op1()], node.get_op2(), cell_size);
            } else {
                heap[data_ptr + node.get_op1()] = subtract(heap[data_ptr + node.get_op1()], node.get_op2(), cell_size);
            }
        } else if (command == "MOVE") {
            if (node.get_op1() >= 0) {
                data_ptr = add(data_ptr, node.get_op1(), heap_size);
            } else {
                data_ptr = subtract(data_ptr, -node.get_op1(), heap_size);
            }
        } else if (command == "JZ") {
            if (heap[data_ptr] == 0) code_ptr = node.get_op1();
        } else if (command == "JNZ") {
            if (heap[data_ptr] != 0) code_ptr = node.get_op1();
        }

        code_ptr += 1;
    }

    return output;
}
