#include "NodeData.h"
#include "AsciiControlCodes.h"
#include <fstream>
#include <iostream>
#include <string>


NodeData::NodeData(std::string& folder, int& id){
    this->filepath = folder + std::to_string(id) + FILE_EXT;
}

NodeData::NodeData(std::string& folder, int& id, std::string& name){
    this->filepath = folder + std::to_string(id) + FILE_EXT;

}

std::string& NodeData::readDataGroup(GroupId group_id){
    int i = group_id;
    return readDataGroup(i);
}

std::string& NodeData::readDataGroup(int& group_id){
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

    return group_str;
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
    int peek = inputStream.peek();

    for(int i = 0; i < 3; i++){
        int size = SIZE_INC;
        int buffer_used = 0;
        char* buffer = (char*) malloc(size);

        // Add to string until end of a group is reached
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
        inputStream.get(c);
        buffer[buffer_used] = 0;
        groups[i] = std::string(buffer);

        // Free resources
        free(buffer);
    }

    // Free resources
    fb.close();

    return groups;
}

bool& NodeData::is_loaded(){
    return dataLoaded;
}