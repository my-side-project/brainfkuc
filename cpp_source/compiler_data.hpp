#include <string>

#pragma once

using namespace std;

namespace compiler_data {
    class Node {
        int line, overload, op1, op2, op3, op4;
        string command;

        public:
        Node(int line, int overload, string command, int op1, int op2, int op3, int op4);

        void set_op1(int op1);
        void set_op2(int op2);
        void set_op3(int op3);
        void set_op4(int op4);
        
        int get_op1();
        int get_op2();
        int get_op3();
        int get_op4();

        int get_line();
        int get_overload();

        string get_command();
    };
}