import brainfuck.util as util


stackable_commands = set(['>', '<', '+', '-'])


def compile(program_text):
    program = util.clean_program(program_text)

    stack = []
    compiled_program = []

    last_instr = None
    last_instr_qnt = 0

    line = 0
    for command in program:

        if command not in stackable_commands and last_instr is not None:
            compiled_program.append(Node(line, last_instr, last_instr_qnt))
            line += 1

            last_instr = None
            last_instr_qnt = 0
        
        if command == '[':
            node = Node(line, command, 1)
            line += 1

            stack.append(node)
            compiled_program.append(node)
        elif command == ']':
            prev_node = stack.pop()
            prev_node.jump_to = line

            compiled_program.append(Node(line, command, 1, jump_to=prev_node.line))
            line += 1
        elif command in stackable_commands:
            if last_instr is None:
                last_instr = command
            
            if last_instr != command:
                compiled_program.append(Node(line, last_instr, last_instr_qnt))
                line += 1

                last_instr = command
                last_instr_qnt = 1
            else:
                last_instr_qnt += 1
        else:
            compiled_program.append(Node(line, command, 1))
            line += 1
    
    return compiled_program


class Node:
    
    def __init__(self, line, command, qnt, jump_to=-1):
        self.line = line
        self.command = command
        self.qnt = qnt
        self.jump_to = jump_to
    
    def __str__(self):
        return f'{self.line}: cmd={self.command}, qnt={self.qnt}, jmp={self.jump_to}'
    
    def __repr__(self):
        return self.__str__()
