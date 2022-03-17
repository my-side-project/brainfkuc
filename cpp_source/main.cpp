#include <string>
#include <vector>
#include <iostream>

#include "interpreter.hpp"

using namespace std;
using namespace interpreter;

int main() {
    string x = "++++++++[>++++ [>++>+++>+++>+<<<<-] >+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    string output = interpret(x);

    cout << output << endl;
}
