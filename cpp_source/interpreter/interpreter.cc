#include <string>
#include <math.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <unordered_map>

#include "../util/util.h"
#include "../compiler/compiler.h"
#include "../compiler/compiler_data.h"
#include "interpreter.h"

using namespace std;
using namespace util;
using namespace compiler;
using namespace compiler_data;

const int cell_size = 256;
const int heap_size = 30000;
const int register_size = 8;

/**
 * Holds state of the running program so it can be passed around.
 */
struct ProgramState {
    int code_ptr;
    int data_ptr;

    int heap[heap_size];
    int registers[register_size];
};

ProgramState get_fresh_program_state() {
    ProgramState state;

    state.code_ptr = 0;
    state.data_ptr = 0;

    for (int i=0; i<heap_size; i++) state.heap[i] = 0;
    for (int i=0; i<register_size; i++) state.registers[i] = 0;

    return state;
}

class CommandRunner {
    public:
    virtual ~CommandRunner() {};
    virtual void execute(Node &command, ProgramState &st) = 0;
};

class PrintCommandRunner: public CommandRunner {
    private:
    bool silent;
    public:
    PrintCommandRunner(bool silent) {
        this->silent = silent;
    }

    void execute(Node &command, ProgramState &st) {
        if (! this->silent) {
            cout<<(char) st.heap[st.data_ptr];
        }
    }
};

class AddCommandRunner: public CommandRunner {
    public:
    void execute(Node &command, ProgramState &st) {
        int o1 = st.heap[st.data_ptr + command.get_op1()];
        
        int o2;
        if (command.get_overload() == 0) o2 = command.get_op2();
        if (command.get_overload() == 1) o2 = st.registers[command.get_op2()];

        st.heap[st.data_ptr + command.get_op1()] = add(o1, o2, cell_size);
    }
};

class MultiplyCommandRunner: public CommandRunner {
    public:
    void execute(Node &command, ProgramState &st) {
        int o1 = st.registers[command.get_op1()];
        int o2 = command.get_op2();
        st.registers[command.get_op1()] = add(0, o1 * o2, cell_size);
    }
};

class DivideCommandRunner: public CommandRunner {
    public:
    void execute(Node &command, ProgramState &st) {
        int o1 = st.registers[command.get_op1()];
        int o2 = command.get_op2();
        bool should_floor = command.get_op3();

        double div = (double) o1 / o2;
        int ans = (int) (should_floor == 1 ? floor(div) : ceil(div));

        st.registers[command.get_op1()] = add(0, ans, cell_size);
    }
};

class CopyCommandRunner: public CommandRunner {
    public:
    void execute(Node &command, ProgramState &st) {
        if (command.get_overload() == 0) {
            st.heap[st.data_ptr + command.get_op1()] = command.get_op2();
        } else if (command.get_overload() == 1) {
            st.heap[st.data_ptr + command.get_op1()] = st.registers[command.get_op2()];
        } else if (command.get_overload() == 2) {
            st.registers[command.get_op1()] = st.heap[st.data_ptr + command.get_op2()];
        } else if (command.get_overload() == 3) {
            st.registers[command.get_op1()] = st.registers[command.get_op2()];
        }
    }
};

class MoveCommandRunner: public CommandRunner {
    public:
    void execute(Node &command, ProgramState &st) {
        int move_by = command.get_op1();
        st.data_ptr = add(st.data_ptr, move_by, heap_size);
    }
};

class JZCommandRunner: public CommandRunner {
    public:
    void execute(Node &command, ProgramState &st) {
        if (st.heap[st.data_ptr + command.get_op1()] == 0) st.code_ptr += command.get_op2();
    }
};

class JNZCommandRunner: public CommandRunner {
    public:
    void execute(Node &command, ProgramState &st) {
        if (st.heap[st.data_ptr + command.get_op1()] != 0) st.code_ptr += command.get_op2();
    }
};

int interpreter::interpret(const vector<Node> &compiled, const bool silent) {
    unordered_map<string, CommandRunner*> command_runners;
    command_runners[CMD_PRINT] = new PrintCommandRunner(silent);
    command_runners[CMD_ADD] = new AddCommandRunner();
    command_runners[CMD_MUL] = new MultiplyCommandRunner();
    command_runners[CMD_DIV] = new DivideCommandRunner();
    command_runners[CMD_COPY] = new CopyCommandRunner();
    command_runners[CMD_MOVE] = new MoveCommandRunner();
    command_runners[CMD_JZ] = new JZCommandRunner();
    command_runners[CMD_JNZ] = new JNZCommandRunner();

    ProgramState state = get_fresh_program_state();

    int instructions = 0;

    while (state.code_ptr < compiled.size()) {
        compiler_data::Node node = compiled[state.code_ptr];
        string command = node.get_command();

        command_runners[command]->execute(node, state);
        state.code_ptr += 1;

        instructions += 1;
    }

    return instructions;
}
