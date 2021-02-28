#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>


class Symbol{
public:
    int occurrences;
    std::string name;
    int value; // relative to the module
};
std::map<std::string, Symbol> symbols; 

int main(int argc, char** argv){
    Symbol sym;
    sym.value = 134;
    std::vector<Symbol*> symbolpointers;
    symbolpointers.push_back(&sym);
    std::cout << symbolpointers[0]->value << std::endl;
    sym.value = 12;
    std::cout << symbolpointers[0]->value << std::endl;

    return 0;
}