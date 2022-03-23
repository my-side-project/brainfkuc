#include <vector>
#include <string>

#include "compiler_data.hpp"

using namespace std;

namespace compiler {
    vector<compiler_data::Node> compile(const string &program_text);

    void print_assembly(vector<compiler_data::Node> compiled);
}
