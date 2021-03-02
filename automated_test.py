# Create a module
import random
import string
import os


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
    return " "
        
MAX_NUM_INST = 10
MAX_NUM_SYM = 10
MAX_LENGTH_SYM = 5
MAX_NUM_USE = 10
letters = string.ascii_lowercase
symbols = [''.join(random.choice(letters) for i in range(random.randint(0, MAX_LENGTH_SYM))) for _ in range(0, MAX_NUM_INST)]

def createModule():
    f = ""
    # definition list
    num_of_symbol_definitions = random.randint(0, len(symbols))
    f += str(num_of_symbol_definitions) + getDelim()
    if num_of_symbol_definitions == " ":
        num_of_symbol_definitions = random.randint(0, MAX_NUM_SYM)
    for i in range(num_of_symbol_definitions):
        symbol = " "
        symbol = random.choice(symbols)
        # if random.randint(0, 10) > 8:
        #     symbol = str(random.randint(0, 10)) + symbol
        operand = ""
        operand = str(random.randint(0, 10000))
        f += symbol + getDelim() + operand + getDelim()
    # uselist
    f += "\n\n"
    num_in_uselist = random.randint(0, MAX_NUM_USE)
    f += str(num_in_uselist) + getDelim()
    if num_in_uselist == " ":
        num_in_uselist = random.randint(0, MAX_NUM_USE)
    for i in range(num_in_uselist):
        symbol = random.choice(symbols)
        f += symbol + getDelim()
    # instructions
    f += "\n\n"
    num_of_inst = random.randint(0, MAX_NUM_INST)
    instructions = ['I', 'A', 'E', 'R', '']
    f += str(num_of_inst) + getDelim()
    if num_of_inst == " ":
        num_of_inst = random.randint(0, MAX_NUM_INST)
    for i in range(num_of_inst):
        ins = random.choice(instructions)
        f += ins + getDelim() + str(random.randint(0, 10))
        if ins == 'R':
            op = random.randint(0, num_in_uselist) if random.randint(0, 10) < 8 else random.randint(0, 10000)
            f += str(op) + getDelim()
        else:
            f += str(random.randint(0, 1000)) + getDelim()
    return f


for i in range(70):
    myString = createModule()
    myString += createModule()
    myString += createModule()
    f = open("demo.txt", "w")
    f.write(myString)
    f.close()
    os.system("./linker demo.txt > out_file_mine")
    os.system("./flinker demo.txt > out_file_his")
    f1 = open("out_file_mine", "r")
    f2 = open("out_file_his", "r")
    assert (f1.read() == f2.read())
    # print("mine: \n", f1.read())
    # print("\n\n\n")
    # print("his: \n", f2.read())
    f1.close()
    f2.close()
