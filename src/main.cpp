#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Node.h"
#include "AsciiControlCodes.h"

int main(int argc, char** argv){
    int SIZE_INC = 10;
    int group_id = 1;
    std::string filepath = "/mnt/c/Users/Joshua/VSCode/SnakeGraphDB/src/text.txt";

    // File buffer to read the file from
    std::filebuf fb;
    
    fb.open(filepath, std::ios::in);

    // Check that the file successfully opened
    if(!fb.is_open()){
        throw std::invalid_argument("Unable to open file");
    }

    // input stream to read the file through
    std::istream inputStream(&fb);

    char c;
    int peek = inputStream.peek();

    // Progress through the file until we are at the desired group
    for(int i = group_id; i--; i > 0){
        while(peek != EOF && peek != A_GROUP_SEP){
            inputStream.get(c);
            peek = inputStream.peek();
        }
        inputStream.get(c);
        peek = inputStream.peek();
    }


    // Initialize buffer
    int size = SIZE_INC;
    int buffer_used = 0;
    char* buffer = (char*) malloc(size);

    // Read the selected group to buffer
    while(peek != EOF && peek != A_GROUP_SEP){
         // If more buffer is full, expand the buffer
        if(buffer_used + 1 >= size){
            size += SIZE_INC;
            buffer = (char*) realloc(buffer, size);
        }
        inputStream.get(c);
        peek = inputStream.peek();
        buffer[buffer_used++] = c;
    }

    // Put buffer into string
    buffer[buffer_used] = 0;
    std::string group_str(buffer);

    // Free resources
    free(buffer);
    fb.close();

    std::cout << group_str << std::endl;
}