#include <vector>
#include <string>

using namespace std;

namespace compiler {

    class Node {
        int line, qnt, jump_to;
        char command;

        public:
        Node(int line, char command, int qnt);

        void set_jump_to(int jump_to);
        
        int get_jump_to();
        int get_line();
        int get_qnt();
        char get_command();
    };

    vector<Node> compile(const string &program_text);
}
