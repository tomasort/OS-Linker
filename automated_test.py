# Create a module
import random
import string


def getDelim():
    r = ""
    for i in range(random.randint(0, 30)):
        x = random.randint(0, 10)
        if x > 7:
            r += "\n"
        elif x < 2:
            r += "\t"
        else:
            r += random.randint(0, 7) * " "
    return r
        
MAX_NUM_INST = 10
MAX_NUM_SYM = 10
MAX_LENGTH_SYM = 5
MAX_NUM_USE = 10
letters = string.ascii_lowercase
symbols = [''.join(random.choice(letters) for i in range(random.randint(0, MAX_LENGTH_SYM))) for _ in range(0, MAX_NUM_INST)]

def createModule():
    f = ""
    # definition list
    num_of_symbol_definitions = random.randint(0, len(symbols)) if random.randint(0, 10) < 8 else " "
    f += str(num_of_symbol_definitions) + random.randint(0, 8)* getDelim()
    if num_of_symbol_definitions == " ":
        num_of_symbol_definitions = random.randint(0, MAX_NUM_SYM)
    for i in range(num_of_symbol_definitions):
        symbol = ""
        if random.randint(0, 10) > 2:
            symbol = random.choice(symbols)
            if random.randint(0, 10) > 8:
                symbol = str(random.randint(0, 10)) + symbol
        operand = ""
        if random.randint(0, 10) > 2:
            operand = str(random.randint(0, 10000000))
        f += symbol + getDelim() + operand + getDelim()
    # uselist
    f += "\n\n"
    num_in_uselist = random.randint(0, MAX_NUM_USE) if random.randint(0, 10) < 8 else " "
    f += str(num_in_uselist) + getDelim()
    if num_in_uselist == " ":
        num_in_uselist = random.randint(0, MAX_NUM_USE)
    for i in range(num_in_uselist):
        symbol = random.choice(symbols)
        f += symbol + getDelim()
    # instructions
    f += "\n\n"
    num_of_inst = random.randint(0, MAX_NUM_INST) if random.randint(0, 10) < 8 else " "
    instructions = ['I', 'A', 'E', 'R', '']
    f += str(num_of_inst) + getDelim()
    if num_of_inst == " ":
        num_of_inst = random.randint(0, MAX_NUM_INST)
    for i in range(num_of_inst):
        ins = random.choice(instructions)
        f += ins + getDelim() + str(random.randint(-1, 12))
        if ins == 'R':
            op = random.randint(0, num_in_uselist) if random.randint(0, 10) < 8 else random.randint(0, 10000)
            f += str(op) + getDelim()
        else:
            f += str(random.randint(0, 100000)) + getDelim()
    return f


myString = createModule()
myString += createModule()
f = open("demo.txt", "w")
f.write(myString)
f.close()