#include <iostream>
#include <string>
#include <fstream>

FILE* fp = NULL; 

char* getToken(){
    size_t BUFFER = 1024;
    char line_buf[1024];
    int eof = 0;
    static int linenum = 0;
    static char* token = NULL;
    static char prev_line[1024];
    // Get the first line
    if (linenum == 0){
        do{
            if (fgets(line_buf, BUFFER, fp)){
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
                    if (fgets(line_buf, BUFFER, fp)){
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
    printf("Token: %d:%ld : %s\n", linenum, token - line_buf + 1, token);
    return token;
}

int main(int argc, char** argv){
    std::string filename = argv[1];
    fp = fopen(filename.c_str(), "r");
    char* tok = NULL;
    while (getToken()){
    }
    int res = fclose(fp); if(!(res == 0)) {
        printf("Unable to close file\n"); }
    return 0;
}