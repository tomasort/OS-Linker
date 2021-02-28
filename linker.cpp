#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <string.h>
#define MAX_NUM_VALUE ((1<<30))
#define LINE_BUFFER 1024

class Symbol{
public:
    int occurrences;
    std::string name;
    int value; // relative to the module
};

class Module{
public: 
    static int module_count;
    int base;
    int module_number;
    int size;
    Module(){
        module_number = ++module_count;
    }

};

FILE* fp = NULL; 
std::map<std::string, Symbol> symbols; 
std::vector<Module> modules;
int Module::module_count = 0;
char line_buf[LINE_BUFFER];
int linenum = 0;
int line_offset = 0;
char* next_token = NULL;
char* current_token = NULL;


char* getToken(){
    //TODO: set two global variables for linenumber and lineoffset for error printing
    int eof = 0;
    char* token = NULL;
    static char prev_line[LINE_BUFFER];
    // Get the first line
    if (linenum == 0){
        do{
            if (fgets(line_buf, LINE_BUFFER, fp)){
                strcpy(prev_line, line_buf);
                linenum++;
                token = strtok(line_buf, " \t\n");
            }else{
                eof = 1;
            }
        }while(!token && !eof);
    }else{
        token = strtok(NULL, " \t\n");
        if (!token){
            // Get next line
                do{
                    if (fgets(line_buf, LINE_BUFFER, fp)){
                        strcpy(prev_line, line_buf);
                        linenum++;
                        token = strtok(line_buf, " \t\n");
                    }else{
                        eof = 1;
                    }
                }while(!token && !eof);
        }
    }
    if(eof){
        // We are at the end of the file
        // Get the previous line and previous token and see if
        std::cout << "File ends at" << linenum << "offset: " << strlen(prev_line) << "\n";
        // printf("The file ends at line: %d, offset : %lu\n", linenum, strlen(prev_line));
        return NULL;
    }
    line_offset = token - line_buf + 1;
    //TODO: store the linenumber and the lineoffset in the global variables
    std::cout << "Token: " << linenum << ":" << line_offset << " : " << token << "\n";
    // printf("Token: %d:%ld : %s\n", linenum, token - line_buf + 1, token);
    return token;
}


// Implement readInt() 
int readInt(){
    char* token = getToken();
    int tok = 0;
    if (!token){
        return 0;
    }
    if (strlen(token) > 4){
        // TODO: add an error saying that the number is too large
        return 9999;
    }
    for (int i = 0; i < strlen(token); i++){
        if(!isdigit(token[i])){
            // Print an error and ... keep going? 
            // I think it must be a syntax error. 
            // TODO: see if a character that is not a number in an int is a syntax error 
            // printf("%c is not a number", token[i]);
            return 9999; // I think I need to return 9,999 if there is a character that is not a number
        }
        tok += ((int)token[i] - 48) * pow(10, strlen(token) - 1 - i);
    }
    return tok;
}
// Ints must be less than MAX_NUM_VALUE, if not then return NOT_A_NUMBER error
// Implement readSymbol()
char* readSymbol(){
    std::cout << "Reading symbols" << "\n";
    char* token = getToken();
    if (!token){
        return NULL;
    }
    if (!isalpha(token[0])){
        return NULL;
    }
    for(int i = 1; i < strlen(token); i++){
        if(!isalnum(token[i])){
            // Print an error
            // TODO: find the error that needs to be here when a symbol is not alpha numeric
            return NULL;
        }
    }
    return token;
}
char* printToken(){
    char* token = getToken();
    for (int i = 0; i <= strlen(token); i++){
        std::cout << token[i];
        if (isdigit(token[i])){
            std::cout << token[i] << " Is a digit" << "\n";
        }
    }
    return token;
}

// Implement readIAER()
char readIAER(){
    char* token = getToken();
    // verify that the token is a char 
    if(strlen(token) > 1){
        // I think it whould be Addressing expected error
        return '\0';
    }
    char addressing_mode = token[0];
    if(!isalpha(addressing_mode)){
        // Addressing expected
        return '\0';
    }
    // TODO: Check that addressing_mode is either I, A, E, or R
    return token[0];
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
    // std::cout << "Parse Error line " << linenum << " offset " << lineoffset << " " << errstr[errcode]; 
}

void createSymbol(char* name, int value){
    // Adds a symbol to the symbol table
    // raise an error if the symbol already exists
    std::string sym_name(name);
    // Check if the symbol has been added
    if(symbols.find(sym_name) == symbols.end()){
        Symbol sym; 
        //TODO: Put this in a constructor
        sym.name = sym_name;
        sym.value = value;
        sym.occurrences = 1;
        symbols[sym_name] = sym;
    }else{
        symbols[sym_name].occurrences++;
    }
}


void pass1(){
    while(!feof(fp)){
        Module current_module;
        modules.push_back(current_module);
        if (Module::module_count == 1){
            current_module.base = 0;
        }else{
            current_module.base = modules[Module::module_count - 1].base + modules[Module::module_count - 1].size;
        }
        int defcount = readInt();
        std::cout << "defcount is " << defcount << "\n";
        for(int i = 0; i < defcount; i++){
            char* sym = readSymbol();
            std::cout << "symbol is " << sym << "\n";
            int val = readInt(); // I think this is just the offset. the value relative to the module
            std::cout << "val is " << val << "\n";
            createSymbol(sym, val); // add the symbol to the table
        }
        std::map<std::string, Symbol>::iterator it;
        for(it = symbols.begin(); it != symbols.end();it++){
            std::cout << it->first << '\t' << it->second.value << std::endl;
        }
        int usecount = readInt();
        for(int i = 0; i < usecount; i++){
            char* sym = readSymbol(); // I think this is just to check for syntax
        }
        int instcount = readInt();
        for(int i = 0; i < instcount; i++){
            char addressmode = readIAER();
            int operand = readInt();
            // various checks ... what checks? 
            // Get opcode and see if it is a single digit
            // Chech that the operand is within range <= 512 if A
            // Probably we need to ckeck if there are more intructions than specified
        }
        // I think we need to record the offset
        current_module.size = instcount;
        std::cout << "Number of modules: " << modules.size() << " and the number of symbols is: " << symbols.size() << std::endl;
    }
}

void pass2(){
    while(true){
        // Create a new module
        int defcount = readInt();
        for(int i = 0; i < defcount; i++){
            char* sym = readSymbol();
            int val = readInt();
            createSymbol(sym, val); // This would have to change
        }
        int usecount = readInt();
        for(int i = 0; i < usecount; i++){
            char* sym = readSymbol();
            // We have to do something here
        }
        int instcount = readInt();
        for(int i = 0; i < instcount; i++){
            char addressmode = readIAER();
            int operand = readInt();
            //This have to change here in pass 2
        }
    }
}

int main(int argc, char** argv){
    if (argc < 2){
        printf("No input file");
        return 1;
    }
    fp = fopen(argv[1], "r");
    //
    // pass1();
    // std::cout << feof(fp) << "\n";
    while(!feof(fp)){
        char* token = readToken();
        std::cout << token << "\n";
    }
    if(fclose(fp)){ 
        printf("Unable to close file\n"); 
    }
    return 0;
}


