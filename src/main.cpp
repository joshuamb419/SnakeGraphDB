#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Node.h"
#include "AsciiControlCodes.h"


int main(int argc, char** argv){
    // File buffer to read the file from
    std::filebuf fb;
    
    fb.open("/mnt/c/Users/Joshua/VSCode/SnakeGraphDB/src/text.txt", std::ios::in);

    // Check that the file successfully opened
    if(!fb.is_open()){
        throw std::invalid_argument("Unable to open file");
    }

    // input stream to read the file through
    std::istream inputStream(&fb);

    // Array to put the three groups that will be read into
    std::string* groups = new std::string[3];

    char c;
    int size_inc = 100;
    for(int i = 0; i < 3; i++){
        int size = size_inc;
        int buffer_used = 0;
        char* buffer = (char*) malloc(size);

        while(inputStream.peek() != EOF && inputStream.peek() != A_GROUP_SEP){
            // std::cout << c;
            if(buffer_used >= size){
                size += size_inc;
                buffer = (char*) realloc(buffer, size);
            }
            inputStream.get(c);
            buffer[buffer_used++] = c;
        }
        inputStream.get(c);
        buffer = (char*) realloc(buffer, buffer_used+1);
        buffer[buffer_used] = 0;
        groups[i] = std::string(buffer);
        free(buffer);
    }

    std::cout << groups[0] << std::endl;
    std::cout << groups[1] << std::endl;
    std::cout << groups[2] << std::endl;
    delete[] groups;
}