#include <vector>
#include <string>

#include "compiler_data.hpp"

using namespace std;

namespace loop_compiler {
    class Loop {
        int start, end;
        vector<Loop> subloops;

        public:
        Loop(int start, int end);

        void add_loop(Loop loop);

        int get_start();
        int get_end();
        vector<Loop> get_subloops();
    };

    vector<compiler_data::Node> compile_flat_loops(vector<compiler_data::Node> source);
}