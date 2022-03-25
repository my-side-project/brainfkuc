#include <string>
#include <math.h>

#include "../util/util.h"
#include "../compiler/compiler.h"
#include "../compiler/compiler_data.h"
#include "interpreter.h"

using namespace std;
using namespace util;
using namespace compiler;
using namespace compiler_data;

const int cell_size = 256;
const int heap_size = 30000;
const int register_size = 8;

string interpreter::interpret(const string &program_txt) {
    vector<compiler_data::Node> compiled = compile(program_txt);

    int code_ptr = 0;
    int data_ptr = 0;

    int heap[heap_size];
    int registers[register_size];

    for (int i=0; i<heap_size; i++) heap[i] = 0;
    for (int i=0; i<register_size; i++) registers[i] = 0;

    string output;

    while (code_ptr < compiled.size()) {
        compiler_data::Node node = compiled[code_ptr];
        string command = node.get_command();

        if (data_ptr < 0) {
            return "";
        }

        if (command == CMD_PRINT) {
            output.push_back((char) heap[data_ptr]);
        }

        if (command == CMD_ADD) {
            int o1, o2;

            switch(node.get_overload()) {
                case 0:
                    o1 = heap[data_ptr + node.get_op1()];
                    o2 = node.get_op2();

                    heap[data_ptr + node.get_op1()] = add(o1, o2, cell_size);
                    break;

                case 1:
                    o1 = heap[data_ptr + node.get_op1()];
                    o2 = registers[node.get_op2()];

                    heap[data_ptr + node.get_op1()] = add(o1, o2, cell_size);
                    break;
            }
        }

        if (command == CMD_MUL) {
            switch(node.get_overload()) {
                case 0:
                    int o1 = registers[node.get_op1()];
                    int o2 = node.get_op2();

                    int mult = o1 * o2;
                    registers[node.get_op1()] = add(0, mult, cell_size);
                    break;
            }
        }

        if (command == CMD_DIV) {
            switch(node.get_overload()) {
                case 0:
                    int o1 = registers[node.get_op1()];
                    int o2 = node.get_op2();
                    int should_floor = node.get_op3();

                    double div = (double) o1 / o2;
                    int ans = (int) (should_floor == 1 ? floor(div) : ceil(div));

                    registers[node.get_op1()] = add(0, ans, cell_size);
                    break;
            }
        }

        if (command == CMD_COPY) {
            switch(node.get_overload()) {
                case 0:
                    heap[data_ptr + node.get_op1()] = node.get_op2();
                    break;

                case 1:
                    heap[data_ptr + node.get_op1()] = registers[node.get_op2()];
                    break;
                
                case 2:
                    registers[node.get_op1()] = heap[data_ptr + node.get_op2()];
                    break;
                
                case 3:
                    registers[node.get_op1()] = registers[node.get_op2()];
                    break;
            }
        }

        if (command == CMD_MOVE) {
            int move_by = node.get_op1();
            data_ptr = add(data_ptr, move_by, heap_size);
        }

        if (command == CMD_JZ) {
            if (heap[data_ptr + node.get_op1()] == 0) code_ptr += node.get_op2();
        }

        if (command == CMD_JNZ) {
            if (heap[data_ptr + node.get_op1()] != 0) code_ptr += node.get_op2();
        }

        code_ptr += 1;
    }

    return output;
}
