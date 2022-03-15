import sys
import source.util as util

from source.compiler import compile

HEAP_SIZE = 30000


class Program:
    
    def __init__(self, program_txt):
        self.program = compile(program_txt)

        self.code_ptr = 0
        self.data_ptr = 0

        self.heap = [0 for i in range(HEAP_SIZE)]

    def run(self):
        output = []

        while self.code_ptr < len(self.program):
            command = self.program[self.code_ptr]

            if command.command == '+':
                self.heap[self.data_ptr] = util.add(self.__current_val(), command.qnt)
            
            elif command.command == '-':
                self.heap[self.data_ptr] = util.subtract(self.__current_val(), command.qnt)
            
            elif command.command == '>':
                self.data_ptr += command.qnt
            
            elif command.command == '<':
                self.data_ptr -= command.qnt

            elif command.command == '.':
                output.append(chr(self.__current_val()))

            elif command.command == '[' and self.__current_val() == 0:
                self.code_ptr = command.jump_to + 1
                continue

            elif command.command == ']' and self.__current_val() != 0:
                self.code_ptr = command.jump_to + 1
                continue

            self.code_ptr += 1

        return output

    def __current_val(self):
        return self.heap[self.data_ptr]
