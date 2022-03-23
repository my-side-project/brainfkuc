#include <vector>
#include <string>
#include <stack>
#include <iostream>
#include <unordered_map>

#include "loop_compiler.hpp"
#include "compiler_data.hpp"

using namespace std;
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

        if (node.get_command() == "JZ") {
            Loop loop = Loop (node.get_line(), node.get_op1());
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
        if (source[i].get_command() == "MOVE") {
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
    return source;
}

vector<compiler_data::Node> loop_compiler::compile_flat_loops(vector<compiler_data::Node> source) {
    vector<Loop> loops = get_loops(source);

    // Collapse all the simple (i.e. start and end at same point) loops.
    vector<Loop> simple_flat_loops = get_simple_flat_loops(loops, source);

    return source;
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
