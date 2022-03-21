#include <vector>
#include <string>

using namespace std;

namespace compiler {

    class Node {
        int line, op1, op2;
        string command;

        public:
        Node(int line, string command, int op1, int op2);

        void set_op1(int op1);
        void set_op2(int op2);
        
        int get_op1();
        int get_op2();

        int get_line();

        string get_command();
    };

    vector<Node> compile(const string &program_text);

    void print_assembly(vector<Node> compiled);
}
