#include <vector>
#include <string>
#include <stack>
#include <unordered_map>

#include "loop_compiler.h"
#include "compiler_data.h"
#include "compiler.h"

using namespace std;
using namespace compiler;
using namespace loop_compiler;
using namespace compiler_data;

vector<Loop> get_loops(vector<compiler_data::Node> source) {
    stack<Loop> stack;
    vector<Loop> output;

    int i = 0;
    for (int i=0; i<source.size(); i++) {
        compiler_data::Node node = source[i];

        while (!stack.empty() != 0 && stack.top().get_end() <= i) {
            Loop loop = stack.top();
            stack.pop();

            if (stack.empty()) {
                output.push_back(loop);
            }
        }

        if (node.get_command() == CMD_JZ) {
            Loop loop = Loop (i, i + node.get_op2());
            if (! stack.empty()) {
                stack.top().add_loop(loop);
            }

            stack.push(loop);
        }
    }

    return output;
}

bool is_loop_simple(vector<compiler_data::Node> source, Loop loop) {
    int loc = 0;

    for (int i=loop.get_start(); i<=loop.get_end(); i++) {
        if (source[i].get_command() == CMD_MOVE) {
            loc += source[i].get_op1();
        }
    }

    return loc == 0;
}

vector<Loop> get_simple_flat_loops(vector<Loop> loops, vector<compiler_data::Node> source) {
    vector<Loop> output;

    for (int i=0; i<loops.size(); i++) {
        if (loops[i].get_subloops().size() > 1) continue;
        if (is_loop_simple(source, loops[i])) output.push_back(loops[i]);
    }

    return output;
}

vector<compiler_data::Node> solve_simple_flat_loop(Loop loop, vector<compiler_data::Node> source) {
    vector<Node> output;
    unordered_map<int, int> node_weights;

    int mem_loc = 0;
    for (int i=loop.get_start(); i<=loop.get_end(); i++) {
        Node node = source[i];

        if (node.get_command() == CMD_ADD && node.get_overload() == 0) {
            node_weights[mem_loc] += node.get_op2();
        } else if (node.get_command() == CMD_MOVE) {
            mem_loc += node.get_op1();
        }
    }

    int decrement = node_weights[0];

    if (decrement >= 0) {
        // This is either an infinite loop or depends on initial value being 0.
        // Either way, we can't optimize it.
        return output;
    }

    decrement = -decrement;

    output.push_back(Node (2, CMD_COPY, 0, 0, -1, -1));
    output.push_back(Node (0, CMD_DIV, 0, decrement, 0, -1));

    unordered_map<int, int>::iterator it;
    for (it = node_weights.begin(); it != node_weights.end(); it++) {
        int mem_loc = it->first;
        int count = it->second;

        // Start location is handled differently.
        if (mem_loc == 0) continue;

        output.push_back(Node (3, CMD_COPY, 1, 0, -1, -1));
        output.push_back(Node (0, CMD_MUL, 1, count, -1, -1));

        // Copy contents of register 0 to a temp register 1
        output.push_back(Node (1, CMD_ADD, mem_loc, 1, -1, -1));
    }

    // Copy contents of register 0 to a temp register 1
    output.push_back(Node (0, CMD_COPY, 0, 0, -1, -1));

    return output;
}

vector<compiler_data::Node> loop_compiler::compile_flat_loops(vector<compiler_data::Node> source) {
    vector<Node> compiled;

    vector<Loop> loops = get_loops(source);
    vector<Loop> simple_flat_loops = get_simple_flat_loops(loops, source);

    int code_ptr = 0;
    int loop_ptr = 0;
    while (code_ptr < source.size()) {
        if (loop_ptr < simple_flat_loops.size()
            && code_ptr == simple_flat_loops[loop_ptr].get_start()) {
            vector<Node> optimized = solve_simple_flat_loop(simple_flat_loops[loop_ptr++], source);

            if (! optimized.empty()) {
                for (int i=0; i<optimized.size(); i++) compiled.push_back(optimized[i]);
                code_ptr += source[code_ptr].get_op2();
            }
        } else {
            compiled.push_back(source[code_ptr]);
        }

        code_ptr += 1;
    }

    return compiled;
}

loop_compiler::Loop::Loop(int start, int end) {
    this->start = start;
    this->end = end;
}

int loop_compiler::Loop::get_start() {
    return this->start;
}

int loop_compiler::Loop::get_end() {
    return this->end;
}

void loop_compiler::Loop::add_loop(Loop loop) {
    this->subloops.push_back(loop);
}

vector<Loop> loop_compiler::Loop::get_subloops() {
    return this->subloops;
}
