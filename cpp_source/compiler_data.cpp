#include "compiler_data.hpp"

compiler_data::Node::Node(int line, int overload, string command, int op1, int op2, int op3, int op4) {
    this->line = line;
    this->overload = overload;
    this->command = command;
    this->op1 = op1;
    this->op2 = op2;
    this->op3 = op3;
    this->op4 = op4;
}

void compiler_data::Node::set_op1(int op1) {
    this->op1 = op1;
}

void compiler_data::Node::set_op2(int op2) {
    this->op2 = op2;
}

void compiler_data::Node::set_op3(int op3) {
    this->op3 = op3;
}

void compiler_data::Node::set_op4(int op4) {
    this->op4 = op4;
}

string compiler_data::Node::get_command() {
    return this->command;
}

int compiler_data::Node::get_line() {
    return this->line;
}

int compiler_data::Node::get_overload() {
    return this->overload;
}

int compiler_data::Node::get_op1() {
    return this->op1;
}

int compiler_data::Node::get_op2() {
    return this->op2;
}

int compiler_data::Node::get_op3() {
    return this->op3;
}

int compiler_data::Node::get_op4() {
    return this->op4;
}
