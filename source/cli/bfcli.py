import sys
import argparse

import brainfuck.brainfuck as bf
import brainfuck.compiler as compiler


def run(file):
    code = get_code(file)
    output = bf.Program(code).run()
    print(output)

def asm(file):
    code = get_code(file)
    compiled = compiler.compile(code)
    print ('\n'.join([str(x) for x in compiled]))


def get_code(file):
    with open(file, 'r') as f:
        return str(f.read())


if __name__ == '__main__':
    main_parser = argparse.ArgumentParser('Brainfuck CLI')
    main_parser.add_argument('--debug', action='store_true', help='print debug information')

    subparsers = main_parser.add_subparsers(dest='command')

    run_parser = subparsers.add_parser('run', help='run a brainfuck file')
    run_parser.add_argument('--file')

    asm_parser = subparsers.add_parser('asm', help='compile to mid-level assembly')
    asm_parser.add_argument('--file')

    args = main_parser.parse_args(sys.argv[1:])

    if args.command == 'run':
        run(args.file)
    elif args.command == 'asm':
        asm(args.file)
