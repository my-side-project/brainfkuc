#include <string>
#include <vector>
#include <iostream>

#include "util.hpp"

using namespace std;
using namespace util;

int main() {
    string x = "+++>--ssd";
    string clean_program = util::clean_program(x);
    cout<<clean_program<<util::add(2,6)<<endl;
}
