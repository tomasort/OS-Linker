#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <string.h>
#include <stdio.h>

#define MAX_NUM_VALUE ((1<<30))
#define LINE_BUFFER 1024

class Symbol{
public:
    int definitions;
    int used_in_program;
    int used_in_module;
    int is_too_large;
    std::string name;
    int value; // relative to the module
};

class Module{
public: 
    int base;
    int module_number;
    int size;
    std::vector<Symbol*> symbols;
    Module(int b, int n){
        base = b;
        module_number = n;
    }
};

FILE* fp = NULL;
char line[LINE_BUFFER];
std::map<std::string, Symbol> symbol_table;
std::vector<Module> modules;
int line_number = 0;
int line_offset = 0;
int line_length = 0;
int last_line_number = 0;
int number_of_instructions = 0;


void __printerror(int errcode){
    static std::string errstr[] = {
        "Absolute address exceeds machine size; zero used",
        "Relative address exceeds module size; zero used",
        "External address exceeds length of uselist; treated as immediate",
        "This variable is multiple times defined; first value used",
        "Illegal immediate value; treated as 9999",
        "Illegal opcode; treated as 9999",
    };
    std::cout << " Error: " << errstr[errcode] << std::endl; 
}

void __parseerror(int errcode){
    static std::string errstr[] = {
        "NUM_EXPECTED",
        "SYM_EXPECTED",
        "ADDR_EXPECTED",
        "SYM_TOO_LONG",
        "TOO_MANY_DEF_IN_MODULE",
        "TOO_MANY_USE_IN_MODULE",
        "TOO_MANY_INSTR",
    };
    std::cout << "Parse Error line " << line_number << " offset " 
    << line_offset << ": " << errstr[errcode] << std::endl; 
}

std::string getToken(){
    // This function reads a single token from a file
    static int current_line_num = 0; 
    static int current_line_offset = 0;
    char* token = NULL;  // Current token 
    // transfer the previous line number and offset number into the global variables
    // line_offset = current_line_offset;
    // Get the first line of the file
    if (current_line_num == 0){
        // Consume every space, \t, and \n until we see a new token or we reach eof
        do{
            if (fgets(line, LINE_BUFFER, fp)){
                line_length = strlen(line);
                current_line_num++;
                token = strtok(line, " \t\n");
            }
        }while(!token && !feof(fp));
    }else{
        token = strtok(NULL, " \t\n");
        if (!token){
            // We need to get the next line
            do{
                if (fgets(line, LINE_BUFFER, fp)){
                    line_length = strlen(line);
                    current_line_num++;
                    token = strtok(line, " \t\n");
                }
            }while(!token && !feof(fp));
        }
    }
    if(!token && feof(fp)){
        line_offset = line_length;
        line_number = current_line_num;
        // Return the empty string when we reach eof
        return "";
    }
    line_offset = token - line + 1;
    line_number = current_line_num;
    std::string current_token(token);
    // std::cout << "Token: " << line_number << ":" << line_offset << " : " << token << "\n";
    return current_token;
}

// std::string getToken(){
//     // This function returns the current token and keeps track of the next token if there is one
//     // It returns an empty line when eof is reached. 
//     static std::string next_token;
//     std::string current_token;
//     // To know we have reached EOF before it is too late 
//     // we read one token ahead 
//     if(line_number == 0){
//         current_token = readToken();
//         next_token = readToken();
//     }else{
//         current_token = next_token;
//         next_token = readToken();
//     }
//     if (next_token.empty() && current_token.empty()){
//         // std::cout << "returning last token" << std::endl;
//         line_offset = line_length;
//     }
//     return current_token;
// }

int readInt(){
    std::string token = getToken();
    if (token.empty() && feof(fp)){
        return -1;
    }
    if (token.length() == 0){
        __parseerror(0);
        exit(0);
    }
    for(int i = 0; i < token.length(); i++){
        if(!isdigit(token[i])){
            __parseerror(0);
            exit(0);
        }
    }
    return stoi(token);
}

std::string readSymbol(){
    std::string token = getToken();
    // std::cout << token << std::endl;
    if (token.length() == 0){
        __parseerror(1);
        exit(0);
    }else if (token.length() > 16){  // Valid symbols can be up to 16 characters.
        __parseerror(3);
        exit(0);
    }
    // Symbols always begin with alpha characters followed by optional alphanumerical characters, i.e.[a-Z][a-Z0-9]*
    if (!isalpha(token[0])){
        __parseerror(1);
        exit(0);
    }
    for(int i = 1; i < token.length(); i++){
        if(!isalnum(token[i])){
            __parseerror(1);
            exit(0);
            return NULL;
        }
    }
    return token;
}

char readIAER(){
    std::string token = getToken();
    if ((token.length() == 0) || (token.length() > 1)){
        __parseerror(2);
        exit(0);
    }
    char addressing_mode = token[0];
    if(!isalpha(addressing_mode)){
        __parseerror(2);
        exit(0);
    }
    // TODO: Check that addressing_mode is either I, A, E, or R
    if((addressing_mode != 'I') && (addressing_mode != 'I') && (addressing_mode != 'I') && (addressing_mode != 'I')){
        __parseerror(2);
        exit(0);
    }
    return addressing_mode;
}

bool createSymbol(std::string name, int value){
    // Adds a symbol to the symbol table
    // raise an error if the symbol already exists
    // Check if the symbol has been added
    std::string sym_name(name);
    if(symbol_table.find(name) == symbol_table.end()){
        Symbol sym;
        sym.name = name;
        sym.value = value;
        sym.definitions = 1;
        sym.used_in_module = 0;
        sym.used_in_program = 0;
        sym.is_too_large = 0;
        symbol_table[name] = sym;
        return true;
    }else{
        // std::cout << "The symbol already exists" << std::endl;
        symbol_table[name].definitions++;
        return false;
    }

}

// Symbol* getSymbol(std::string symbol_name){
//     for (int i = 0; i < symbols.size(); i++){
//         if(symbols[i].name = symbol_name){
//             return &symbols[i];
//         }
//     }
// }


void pass1(){
    // Pass1 determines the base address for each module and the absolute address for each defined symbol. 
    int module_offset = 0;
    std::vector<Symbol*> defined_symbols; // this is to keep the order in which the symbols where defined
    while(!feof(fp)){
        Module current_module(module_offset, modules.size()); 
        int defcount = readInt();
        if (defcount < 0 && feof(fp)){
            break;
        }
        if (defcount > 16){  // We can't have more than 16 symbols
            __parseerror(4);
            exit(0);
        }
        for (int i = 0; i < defcount; i++){  // Parse the symbols and their values
            std::string sym = readSymbol();
            int val = module_offset + readInt();
            if(createSymbol(sym, val)){ // add the symbol to symbol_table
                defined_symbols.push_back(&symbol_table[sym]);
            }
            current_module.symbols.push_back(&symbol_table[sym]);
        }
        int usecount = readInt();
        if (usecount > 16){
            __parseerror(5);
            exit(0);
        }
        for (int i = 0; i < usecount; i++){
            std::string sym = readSymbol();
        }
        int instcount = readInt();
        number_of_instructions += instcount;
        if (number_of_instructions > 512){
            __parseerror(6);
            exit(0);
        }
        for (int i = 0; i < instcount; i++){
            char addressing_mode = readIAER();
            // If addressing_mode is A, check that operand is less than 512
            int operand = readInt();
            // various checks ... what checks? 
        }
        // Check if the values of symbols in the module are larger than the size of the module
        for (int i = 0; i < current_module.symbols.size(); i ++){
            Symbol* sym = current_module.symbols[i];
            if ((sym->value - module_offset) >= instcount){
                std::cout << "Warning: Module "
                << modules.size() + 1 << ": " << sym->name << " too big " 
                << sym->value - module_offset << " (max=" << instcount - 1
                << ") assume zero relative" << std::endl;
                symbol_table[current_module.symbols[i]->name].value = module_offset;
            } 
        }
        module_offset += instcount; 
        current_module.size = instcount;
        modules.push_back(current_module);
    }
    std::cout << "Symbol Table" << std::endl;
    for (int i = 0; i < defined_symbols.size(); i++){
        Symbol* sym = defined_symbols[i];
        std::cout << sym->name << '=' << sym->value;
        if (sym->definitions > 1){
            __printerror(3);
            sym-> definitions = 1;
        }else{
            std::cout << "\n";
        }
    }
    // std::cout << "Symbol Table" << std::endl;
    // std::map<std::string, Symbol>::iterator it;
    // for(it = symbol_table.begin(); it != symbol_table.end();it++){
    //     std::cout << it->first << '=' << it->second.value;
    //     if (it->second.definitions > 1){
    //         __printerror(3);
    //     }else{
    //         std::cout << "\n";
    //     }
    // }
}

std::string printOperand(int opcode, int operand){
    std::string op = std::to_string(operand);
    while (op.size() < 3){
        op = "0" + op;
    }
    return std::to_string(opcode) + op;
}

std::string printInstNum(int num){
    std::string linenum = std::to_string(num);
    while (linenum.size() < 3){
        linenum = "0" + linenum;
    }
    return linenum;
}

void pass2(){
    int module_number = 0;
    int instruction_number = 0;
    int module_base = 0;
    std::cout << "Memory Map" << std::endl;
    while(!feof(fp)){
        int defcount = readInt();
        if (defcount < 0 && feof(fp)){
            break;
        }
        for (int i = 0; i < defcount; i++){
            std::string sym = readSymbol();
            int val = readInt();
        }
        std::vector<std::string> uselist;
        int usecount = readInt();
        for (int i = 0; i < usecount; i++){
            std::string sym = readSymbol();
            uselist.push_back(sym);
            if (symbol_table.find(sym) == symbol_table.end()){
                // The symbol in the uselist is not defined in the program
                Symbol s;
                s.definitions = 0;
                s.used_in_module = 0;
                s.used_in_program = 0;
                symbol_table[sym] = s;
            }
        }
        int instcount = readInt();
        for (int i = 0; i < instcount; i++){
            char addressing_mode = readIAER();
            int operand = readInt();
            std::cout << printInstNum(instruction_number) << ": ";
            if (operand/1000 >= 10 && addressing_mode != 'I'){
                std::cout << printOperand(9, 999);
                __printerror(5);
            }else{
                switch(addressing_mode){
                    case 'I':
                        if (operand >= 10000){
                            std::cout << printOperand(9, 999);
                            __printerror(4);
                        }else{
                            std::cout << printOperand(operand/1000, operand%1000) << std::endl;
                        }
                        break;
                    case 'A':
                        if (operand%1000 >= 512){  // it can't be ">=" the machine size 512
                            std::cout << printOperand(operand/1000, 0);
                            __printerror(0);
                        }else{
                            std::cout << printOperand(operand/1000, operand%1000) << std::endl;
                        }
                        break;
                    case 'E':
                        if (operand%1000 >= usecount){
                            std::cout << printOperand(operand/1000, operand%1000);
                            __printerror(2);
                        }else{
                            if ((symbol_table.find(uselist[operand%1000]) != symbol_table.end()) && (symbol_table[uselist[operand%1000]].definitions > 0)){
                                std::cout << printOperand(operand/1000, symbol_table[uselist[operand%1000]].value) << std::endl;
                            }else{
                                std::cout << printOperand(operand/1000, 0);
                                std::cout << " Error: " << uselist[operand%1000] << " is not defined; zero used" << std::endl;
                            }
                            if (symbol_table.find(uselist[operand%1000]) != symbol_table.end()){
                                symbol_table[uselist[operand%1000]].used_in_module++;
                                symbol_table[uselist[operand%1000]].used_in_program++;
                            }
                        }
                        break;
                    case 'R':
                        if ((operand%1000+module_base) > (module_base+instcount)){
                            std::cout << printOperand(operand/1000, module_base); // keep only the opcode
                            __printerror(1);
                        }else{
                            std::cout << printOperand(operand/1000, operand%1000 + module_base) << std::endl;
                        }
                        break;
                }
            }
            instruction_number++;
        }
        module_base += instcount;
        for (int i = 0; i < usecount; i++){
            if(symbol_table.find(uselist[i]) != symbol_table.end()){
                Symbol* sym = &symbol_table[uselist[i]];
                if (sym->used_in_module == 0){
                    std::cout << "Warning: Module " << module_number + 1 << ": " << uselist[i]
                    <<  " appeared in the uselist but was not actually used\n";
                }
            }else{
                continue;
            }
        }
        // reset the used_in_module counts for each symbol in the uselist
        for (int i = 0; i < usecount; i++){
            if(symbol_table.find(uselist[i]) != symbol_table.end()){
                Symbol* sym = &symbol_table[uselist[i]];
                sym->used_in_module = 0;
            }
        }
        module_number++;
    }
    std::cout << "\n";
    for(int i = 0; i < module_number; i++){
        for (int j = 0; j < modules[i].symbols.size(); j++){
            if (modules[i].symbols[j]->used_in_program == 0){
                std::cout << "Warning: Module "
                << i + 1 << ": " << modules[i].symbols[j]->name << " was defined but never used\n";
                modules[i].symbols[j]->used_in_program = 1;
            } 
        }
    }
}

int main(int argc, char** argv){
    if (argc < 2){
        printf("No input file");
        return 1;
    }
    fp = fopen(argv[1], "r");
    // Go through pass 1
    pass1();
    // if(fclose(fp)){
    //     printf("Unable to close file\n"); 
    // }
    // fp = fopen(argv[1], "r");
    rewind(fp);
    std::cout << "\n";
    pass2();
    std::cout << "\n";
    // Go through pass 2
    if(fclose(fp)){ 
        printf("Unable to close file\n"); 
    }
    return 0;
}
