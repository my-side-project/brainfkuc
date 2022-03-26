#ifndef COMPILER_COMPILER_DATA_
#define COMPILER_COMPILER_DATA_

#include <string>
using namespace std;

namespace compiler_data {
    class Node {
        int overload, op1, op2, op3, op4;
        string command;

        public:
        Node(int overload, string command) {
            init(overload, command, -1, -1, -1, -1);
        }

        Node(int overload, string command, int op1) {
            init(overload, command, op1, -1, -1, -1);
        }

        Node(int overload, string command, int op1, int op2) {
            init(overload, command, op1, op2, -1, -1);
        }

        Node(int overload, string command, int op1, int op2, int op3) {
            init(overload, command, op1, op2, op3, -1);
        }

        Node(int overload, string command, int op1, int op2, int op3, int op4) {
            init(overload, command, op1, op2, op3, op4);
        }

        void init(int overload, string command, int op1, int op2, int op3, int op4) {
            this->overload = overload;
            this->command = command;
            this->op1 = op1;
            this->op2 = op2;
            this->op3 = op3;
            this->op4 = op4;
        }

        void set_op1(int op1) {
            this->op1 = op1;
        }

        void set_op2(int op2) {
            this->op2 = op2;
        }

        void set_op3(int op3) {
            this->op3 = op3;
        }

        void set_op4(int op4) {
            this->op4 = op4;
        }

        int get_op1() {
            return this->op1;
        }

        int get_op2() {
            return this->op2;
        }

        int get_op3() {
            return this->op3;
        }

        int get_op4() {
            return this->op4;
        }

        int get_overload() {
            return this->overload;
        }

        string get_command() {
            return this->command;
        }
    };
}

#endif