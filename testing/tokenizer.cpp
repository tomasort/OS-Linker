#include <iostream>
#include <string>
#include <map>
#include <vector>

#define MAX_NUM_VALUE ((1<<30))
#define LINE_BUFFER 1024

FILE* fp = NULL;
int line_number = 0;
int line_offset = 0;
int line_length = 0;
int last_line_number = 0;
char line[LINE_BUFFER];

std::string readToken(){
    // This function reads a single token from a file
    static int current_line_num = 0; 
    static int current_line_offset = 0;
    char* token = NULL;  // Current token 
    // transfer the previous line number and offset number into the global variables
    line_number = current_line_num;
    line_offset = current_line_offset;
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
    if(feof(fp)){
        last_line_number = current_line_num;
        // Return the empty string when we reach eof
        return "";
    }
    current_line_offset = token - line + 1;
    std::string current_token(token);
    return current_token;
}

std::string getToken(){
    // This function returns the current token and keeps track of the next token if there is one
    // It returns an empty line when eof is reached. 
    static std::string next_token;
    std::string current_token;
    // To know we have reached EOF before it is too late 
    // we read one token ahead 
    if(line_number == 0){
        current_token = readToken();
        next_token = readToken();
    }else{
        current_token = next_token;
        next_token = readToken();
    }
    return current_token;
}

int main(int argc, char** argv){
    if (argc < 2){
        printf("No input file");
        return 1;
    }
    fp = fopen(argv[1], "r");
    while(!feof(fp)){
        std::string token = getToken();
        std::cout << "Token: " << line_number << ":" << line_offset << " : " << token << "\n";
    }
    std::cout << "File ends at: " << last_line_number << " offset: " << line_length << "\n";
    if(fclose(fp)){ 
        printf("Unable to close file\n"); 
    }
    return 0;
}
