import unittest
import source.brainfuck as bf

class BrainfuckTest(unittest.TestCase):

    def test_add(self):
        out = run('''
            ++ > +++++
            [ < + > - ]
            ++++ ++++
            [ < +++ +++ > - ]
            < .
            ''')
        self.assertEqual(out[0], '7')
    
    def test_hello_world(self):
        out = run('''
            ++++++++[>++++ [>++>+++>+++>+<<<<-] >+>+>->>+[<]<-]
            >>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.
            ''')
        self.assertEqual(''.join(out).strip(), 'Hello World!')

    def test_hello_world_2(self):
        out = run('''
            >++++++++[-<+++++++++>]<.>>+>-[+]++>++>+++[>[->+++<<+++>]<<]>-----.>->
            +++..+++.>-.<<+[>[+>+]>>]<--------------.>>.+++.------.--------.>+.>+.
        ''')
        self.assertEqual(''.join(out).strip(), 'Hello World!')
    
    def test_hello_world_3(self):
        out = run('''
            +[+[<<<+>>>>]+<-<-<<<+<++]<<.<++.<++..+++.<<++.<---.>>.>.+++.------.>-.>>--.
        ''')
        self.assertEqual(''.join(out).strip(), 'Hello, World!')
    
    def cell_width(self):
        out = run('''
            Calculate the value 256 and test if it's zero
            If the interpreter errors on overflow this is where it'll happen
            ++++++++[>++++++++<-]>[<++++>-]
            +<[>-<
                Not zero so multiply by 256 again to get 65536
                [>++++<-]>[<++++++++>-]<[>++++++++<-]
                +>[>
                    # Print "32"
                    ++++++++++[>+++++<-]>+.-.[-]<
                <[-]<->] <[>>
                    # Print "16"
                    +++++++[>+++++++<-]>.+++++.[-]<
            <<-]] >[>
                # Print "8"
                ++++++++[>+++++++<-]>.[-]<
            <-]<
            # Print " bit cells\n"
            +++++++++++[>+++>+++++++++>+++++++++>+<<<<-]>-.>-.+++++++.+++++++++++.<.
            >>.++.+++++++..<-.>>-
            Clean up used cells.
            [[-]<]
        ''')
        self.assertEqual(''.join(out).strip(), '8 bit cells')

def run(code):
    return bf.Program(code).run()
