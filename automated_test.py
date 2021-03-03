#!/usr/bin/python
# -*- coding: utf-8 -*-
# Create a module

import random
import string
import os
import time


def getDelim():
    r = ' '
    for i in range(random.randint(0, 5)):
        x = random.randint(0, 10)
        if x > 8:
            r += '\n'
        elif x < 1:
            r += '\t'
        else:
            r += random.randint(0, 5) * ' '
    return r


symbols = []


def createModule():
    MAX_NUM_INST = random.randint(40, 513)
    MAX_NUM_SYM = random.randint(5, 17)
    MAX_LENGTH_SYM = random.randint(1, 5)
    MAX_NUM_USE = random.randint(6, 17)
    MAX_OPERAND = random.randint(910, 1020)
    f = ''

    # definition list

    letters = string.ascii_lowercase + ''
    if random.randint(0, 10) <= 1:
        my_letters = string.ascii_lowercase + '3 '
    num_of_symbol_definitions = random.randint(3, MAX_NUM_SYM)
    num_of_inst = random.randint(0, MAX_NUM_INST)
    f += str(num_of_symbol_definitions) + getDelim()
    for _ in range(num_of_symbol_definitions):
        symbol = ''.join(random.choice(letters) for i in
                         range(random.randint(0, MAX_LENGTH_SYM)))
        symbols.append(symbol)
        operand = str(random.randint(0, 20))
        f += symbol + getDelim() + operand + getDelim()

    # uselist

    f += '''

'''
    num_in_uselist = random.randint(0, MAX_NUM_USE)
    f += str(num_in_uselist) + getDelim()
    for i in range(num_in_uselist):
        symbol = random.choice(symbols)
        f += symbol + getDelim()

    # instructions

    f += '''

'''
    instructions = ['I', 'A', 'E', 'R']
    if random.randint(0, 10) <= 1:
        instructions = [
            'I',
            'P',
            'E',
            'R',
            'F',
            ' ',
            ]
    f += str(num_of_inst)
    if random.randint(0, 10) > 8:
        f += str(random.randint(0, 100))
    f += getDelim()
    for i in range(num_of_inst):
        ins = random.choice(instructions)
        f += ins + getDelim() + str(random.randint(0, 11))
        if ins == 'R':
            op = (random.randint(0, int(num_in_uselist
                  * 1.50)) if random.randint(0, 10)
                  < 8 else random.randint(0, 100000))
            f += str(op) + getDelim()
        else:
            f += str(random.randint(0, MAX_OPERAND)) + getDelim()
    p = string.ascii_lowercase + string.ascii_uppercase \
        + string.punctuation
    for i in range(0, random.randint(0, 1)):
        x = random.randint(0, len(f))
        f = ''.join((f[:x], random.choice(p), f[x:]))
    return f


def createModule2():
    MAX_NUM_INST = 10
    MAX_NUM_SYM = 10
    MAX_LENGTH_SYM = 5
    MAX_NUM_USE = 1
    letters = string.ascii_lowercase
    symbols = [''.join(random.choice(letters) for i in
               range(random.randint(0, MAX_LENGTH_SYM))) for _ in
               range(0, MAX_NUM_INST)]
    f = ''

    # definition list

    num_of_symbol_definitions = random.randint(0, len(symbols))
    f += str(num_of_symbol_definitions) + getDelim()
    if num_of_symbol_definitions == ' ':
        num_of_symbol_definitions = random.randint(0, MAX_NUM_SYM)
    for i in range(num_of_symbol_definitions):
        symbol = ' '
        symbol = random.choice(symbols)

        # if random.randint(0, 10) > 8:
        #     symbol = str(random.randint(0, 10)) + symbol

        operand = ''
        operand = str(random.randint(0, 10))
        f += symbol + getDelim() + operand + getDelim()

    # uselist

    f += '''

'''
    num_in_uselist = random.randint(0, MAX_NUM_USE)
    f += str(num_in_uselist) + getDelim()
    if num_in_uselist == ' ':
        num_in_uselist = random.randint(0, MAX_NUM_USE)
    for i in range(num_in_uselist):
        symbol = random.choice(symbols)
        f += symbol + getDelim()

    # instructions

    f += '''

'''
    num_of_inst = random.randint(0, MAX_NUM_INST)
    instructions = ['I', 'A', 'E', 'R', '']
    f += str(num_of_inst) + getDelim()
    if num_of_inst == ' ':
        num_of_inst = random.randint(0, MAX_NUM_INST)
    for i in range(num_of_inst):
        ins = random.choice(instructions)
        f += ins + getDelim() + str(random.randint(0, 10))
        if ins == 'R':
            op = (random.randint(0,
                  num_in_uselist) if random.randint(0, 10)
                  < 8 else random.randint(0, 10000))
            f += str(op) + getDelim()
        else:
            f += str(random.randint(0, 1000)) + getDelim()
    return f

    
#!/usr/local/bin/python3
import random
import string

def test_prob(prob = 1.0):
    return random.random() > prob

def sample(list):
    return random.sample(list, 1)[0]

Characters = list(string.ascii_lowercase + string.ascii_uppercase) + list(str(i) for i in range(10))

def generate_symbol(symbol_table):
    if len(symbol_table) > 0 and not test_prob(SemanticErrorProb):
        return sample(list(symbol_table))
    else:
        first_letter = sample(string.ascii_lowercase)
        symbol_length = random.randint(0, 15) if test_prob(SyntaxErrorProb) else random.randint(0, 50)
        return first_letter + ''.join(sample(Characters) for _ in range(symbol_length))

Types = ['I', 'A', 'R', 'E']

def generate_instr_type():
    return sample(Types) if test_prob(SyntaxErrorProb) else sample(string.ascii_uppercase)

def generate_instr_code(instr_type, module_size, use_list):
    op_code = str(random.randint(0, 9)) if test_prob(SyntaxErrorProb) else ''
    if instr_type == 'R':
        if module_size > 0 and test_prob(SemanticErrorProb):
            relative_addr = random.randint(0, module_size - 1)
        else:
            relative_addr = random.randint(0, 999)
        return op_code + '{0:03}'.format(relative_addr)
    elif instr_type == 'E':
        if len(use_list) > 0 and test_prob(SemanticErrorProb):
            use_index = random.randint(0, len(use_list) - 1)
        else:
            use_index = random.randint(0, 30)
        return op_code + '{0:03}'.format(use_index)
    else:
        return op_code + str(random.randint(0, 512) if test_prob(SemanticErrorProb) else random.randint(0, 10000))

Whitespaces = [' ', '\t', '\n']

def write_token(file, token, max_number_of_whitespaces = 10):
    file.write(str(token))
    file.write(' ')
    """
    for _ in range(random.randint(1, max_number_of_whitespaces + 1)):
        file.write(sample(Whitespaces))
    """

def generate_module(file, symbol_table, instr_count):
    number_of_defs = random.randint(0, 16) if test_prob(SyntaxErrorProb) else random.randint(0, 30)
    number_of_uses = random.randint(0, 16) if test_prob(SyntaxErrorProb) else random.randint(0, 30)
    if test_prob(SyntaxErrorProb):
        number_of_instrs = 0 if instr_count >= 512 else random.randint(0, 512 - instr_count)
    else:
        number_of_instrs = random.randint(0, 512)

    write_token(file, number_of_defs)
    for _ in range(number_of_defs):
        token = generate_symbol(symbol_table)
        if number_of_instrs > 0 and test_prob(SyntaxErrorProb):
            offset = random.randint(0, number_of_instrs - 1)
        else:
            offset = random.randint(0, 512)
        if token not in symbol_table:
            symbol_table[token] = offset
        write_token(file, token)
        write_token(file, offset)
    file.write('\n')

    write_token(file, number_of_uses)
    use_list = []
    for _ in range(number_of_uses):
        token = sample(list(symbol_table)) if test_prob(SemanticErrorProb) else generate_symbol(symbol_table)
        use_list.append(token)
        write_token(file, token)
    file.write('\n')

    write_token(file, number_of_instrs)
    for _ in range(number_of_instrs):
        instr_type = generate_instr_type()
        instr_code = generate_instr_code(instr_type, number_of_instrs, use_list)
        write_token(file, instr_type)
        write_token(file, instr_code)
    file.write('\n')

    return number_of_instrs

def generate_input(path):
    fout = open(path, 'w')
    symbol_table = {}
    number_of_modules = random.randint(1, MaxNumberOfModules)
    instr_count = 0

    for i in range(number_of_modules):
        instr_count += generate_module(fout, symbol_table, instr_count)

    fout.close()

SyntaxErrorProb = 0.00
SemanticErrorProb = 0.05
MaxNumberOfModules = 20

print 'Starting testing'
time.sleep(2)
for i in range(1, 21):
    os.system('./linker lab1samples/input-' + str(i)
              + ' > out_file_mine' + '&& cat out_file_mine')
    os.system('/home/frankeh/Public/linker lab1samples/input-' + str(i)
              + ' > out_file_his')
    f1 = open('out_file_mine', 'r')
    f2 = open('out_file_his', 'r')
    s1 = f1.read()
    s2 = f2.read()
    print("Verify " + str(i)) 
    assert(s1 == s2)
    print("File " + str(i) + "Completed")
    f1.close()
    f2.close()
time.sleep(2)
print 'Starting random testing'
for i in range(1000):
    try:
        generate_input("demo.txt")
    except ValueError:
        print("Some Error")
        continue
    os.system('./linker demo.txt > out_file_mine && cat out_file_mine')
    os.system('/home/frankeh/Public/linker demo.txt > out_file_his')
    f1 = open('out_file_mine', 'r')
    f2 = open('out_file_his', 'r')
    s1 = f1.read()
    s2 = f2.read()
    assert s1 == s2
print 'Starting random testing'
time.sleep(1)
for i in range(10000):
    myString = getDelim()
    if random.randint(0, 10) >= 2:
        if random.randint(0, 10) >= 5:
            myString += createModule2()
        if random.randint(0, 10) >= 5:
            myString += createModule()
        if random.randint(0, 10) > 7:
            myString += createModule()
        if random.randint(0, 10) > 8:
            myString += '\n'
        if random.randint(0, 10) > 5:
            myString += createModule2()
        if random.randint(0, 10) > 7:
            myString += createModule()
        if random.randint(0, 10) > 8:
            myString += '\n'
    myString += getDelim()
    f = open('demo.txt', 'w')
    f.write(myString)
    f.close()
    os.system('./linker demo.txt > out_file_mine && cat out_file_mine')
    os.system('/home/frankeh/Public/linker demo.txt > out_file_his')
    f1 = open('out_file_mine', 'r')
    f2 = open('out_file_his', 'r')
    s1 = f1.read()
    s2 = f2.read()
    assert s1 == s2
    print '''

'''

    # print("mine: \n", f1.read())
    # print("\n\n\n")
    # print("his: \n", f2.read())

    f1.close()
    f2.close()

            
