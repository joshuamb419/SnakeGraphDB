#include "Node.h"
#include "AsciiControlCodes.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>

Node::Node(std::string& folder, int& id){
    this->id = id;
    this->filepath = folder + std::to_string(id) + FILE_EXT;

    // Pull the name from the file
    std::string idGroup = read_data_group(Node::identification);
    this->name = idGroup.substr(idGroup.find(A_RECORD_SEP) + 1);

    // Find out how many nodes reference this node
    std::string connectionGroup = read_data_group(Node::connections);
    int connectionSplit = connectionGroup.find(A_RECORD_SEP);
    this->reference_count = atoi(connectionGroup.substr(0, connectionSplit).c_str());

    // Get a list of the nodes this node references
    std::string connectionListString = connectionGroup.substr(connectionSplit + 1);
    int split_id;
    while(connectionListString.length() > 0){
        split_id = connectionListString.find(A_UNIT_SEP);

        // add id to the vector
        if(split_id == std::string::npos){
            split_id = connectionListString.length() - 1;
            connection_ids.push_back(atoi(connectionListString.c_str()));
        } else {
            connection_ids.push_back(atoi(connectionListString.substr(0, split_id).c_str()));
        }

        // remove the processed element form the string
        connectionListString.erase(split_id + 1);
        
    }
}

Node::Node(std::string& folder, int& id, std::string& name){
    this->id = id;
    this->name = name;
    this->reference_count = 0;
    this->filepath = folder + std::to_string(id) + FILE_EXT;
}

int& Node::get_id(){
    return id;
}

std::string& Node::get_name(){
    return name;
}

std::vector<int>& Node::get_connection_ids(){
    return connection_ids;
}

void Node::add_reference(){
    reference_count++;
}

int& Node::get_reference_count(){
    return reference_count;
}

void Node::remove_reference(){
    if(--reference_count == 0){
        delete this;
    }
}

void Node::add_connection(int& targetId){
    if(std::find(connection_ids.begin(), connection_ids.end(), targetId) == connection_ids.end()){
        connection_ids.push_back(targetId);
    }
}

void Node::remove_connection(int& targetId){
    auto it = std::find(connection_ids.begin(), connection_ids.end(), targetId);
    if(it != connection_ids.end()){
        connection_ids.erase(it);
    }
}

std::string& Node::read_data_group(GroupId group_id){
    int i = group_id;
    return read_data_group_id(i);
}

std::string& Node::read_data_group_id(int& group_id){
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

std::string* Node::read_data_groups(){
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

bool& Node::is_loaded(){
    return data_loaded;
}

void Node::load_data(){
    std::string data_str = read_data_group(Node::data);
    
    // Don't try to load if there is no data
    if(data_str.length() == 0) return;
    // If data is already loaded, do not reload
    if(data_loaded) return;

    // Initialize the map
    node_contents = new std::unordered_map<std::string, std::string>();

    int split_id;
    std::string data_element;
    std::string key;
    std::string value;
    while(data_str.length() > 0){
        // Find next seperator
        split_id = data_str.find(A_RECORD_SEP);

        // If there is no seperator found we are on the last element
        if(split_id == std::string::npos){
            split_id = data_str.length() - 1;
            data_element = data_str;
        // If the seperator was found read to the seperator
        } else {
            data_element = data_str.substr(0, split_id);
        }
        // Remove the element from the data string
        data_str.erase(0, split_id + 1);

        // Seperate the element into its key/value pair
        int seperator_pos = data_element.find(A_UNIT_SEP);
        key = data_element.substr(0, seperator_pos);
        value = data_element.substr(seperator_pos + 1);

        node_contents->emplace(key, value);
    }

    data_loaded = true;
}


Node::~Node(){
    write_data();
    delete node_contents;
}
