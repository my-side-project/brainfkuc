VALID_COMMANDS = set(['>', '<', '+', '-', '.', ',', '[', ']'])


def add(val, to_add):
    return (val + to_add) % 256


def subtract(val, to_subtract):
    diff = val - to_subtract

    if diff >= 0:
        return diff
    
    return subtract(255, to_subtract - val - 1)


def clean_program(program_text):
    return [x for x in program_text if x in VALID_COMMANDS]
