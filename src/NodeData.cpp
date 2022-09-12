#include "NodeData.h"
#include "AsciiControlCodes.h"
#include <fstream>
#include <iostream>
#include <string>


NodeData::NodeData(std::string& folder, int* id){
    this->filepath = folder + std::to_string(*id) + FILE_EXT;
}

NodeData::NodeData(std::string& folder, int* id, std::string& name){

}

std::string* NodeData::readDataGroups(){
    // File buffer to read the file from
    std::filebuf fb;
    
    fb.open(filepath, std::ios::in);

    // Check that the file successfully opened
    if(!fb.is_open()){
        throw std::invalid_argument("Unable to open file");
    }

    // input stream to read the file through
    std::istream inputStream(&fb);

    // Array to put the three groups that will be read into
    std::string* groups = new std::string[3];

    char c;
    // How many bytes will be added to the buffer every time more space is needed
    int size_inc = 100;
    for(int i = 0; i < 3; i++){
        int size = size_inc;
        int buffer_used = 0;
        char* buffer = (char*) malloc(size);

        // Add to string until end of a group is reached
        while(inputStream.peek() != EOF && inputStream.peek() != A_GROUP_SEP){
            // If more buffer is full, expand the buffer
            if(buffer_used >= size){
                size += size_inc;
                buffer = (char*) realloc(buffer, size);
            }
            inputStream.get(c);
            buffer[buffer_used++] = c;
        }
        inputStream.get(c);
        buffer[buffer_used] = 0;
        groups[i] = std::string(buffer);
        free(buffer);
    }

    return groups;
}