#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#define MAX_NUM_VALUE ((1<<30))
#define LINE_BUFFER 1024


/* 
    make sure the last location of the file
    which you read when getToken() returns NULL (no token anymore)  
    is actually at the end of the last line not the line you read which was then empty.
    always do "wc -l <file>" (linecount)  and that should agree with your last reported line.
*/

FILE* fp = NULL; 

char* getToken(){
    //TODO: set two global variables for linenumber and lineoffset for error printing
    char line_buf[LINE_BUFFER];
    int eof = 0;
    char* token = NULL;
    static int linenum = 0;
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
        printf("The file ends at line: %d, offset : %lu\n", linenum, strlen(prev_line));
        return NULL;
    }
    //TODO: store the linenumber and the lineoffset in the global variables
    printf("Token: %d:%ld : %s\n", linenum, token - line_buf + 1, token);
    return strdup(token);
}

// Implement readInt() 
int readInt(){
    char* token = getToken();
    printf("verifying the string %s is a number =< 9,999\n", token);
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
            printf("%c is not a number", token[i]);
            return 9999; // I think I need to return 9,999 if there is a character that is not a number
        }
        printf("%c\n", token[i]);
    }
    return atoi(token);
}
// Ints must be less than MAX_NUM_VALUE, if not then return NOT_A_NUMBER error
// Implement readSymbol()
char *readSymbol(){
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

// Implement readIAER()
char readIAER(){
    char* token = getToken();
    // verify that the token is a char 
    if(strlen(token) > 1){
        return NULL;
    }
    return 'A';
}

int main(int argc, char** argv){
    fp = fopen(argv[1], "r");
    // int testint = readInt();
    // printf("this is an int %d\n", testint);
    getToken();
    char* testsymbol = readSymbol();
    printf("This is a symbol %s\n", testsymbol);
    char testAddress = readIAER();
    printf("This is an addressing mode %c", testAddress);
    if(fclose(fp)){ 
        printf("Unable to close file\n"); 
    }
    return 0;
}


