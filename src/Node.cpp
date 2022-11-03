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
}

Node::Node(std::string& folder, int& id, std::string& name){
    Node(folder, id, name, true);
}

Node::Node(std::string& folder, int& id, std::string& name, bool overwrite){
    this->id = id;
    this->name = name;
    this->filepath = folder + std::to_string(id) + FILE_EXT;
    data_changed = true;
    if(overwrite) {
        data_loaded = true;
        delete_node();
    }
    write_data();
}

int& Node::get_id(){
    return id;
}

std::string& Node::get_name(){
    return name;
}

std::string Node::read_data_group(GroupId group_id){
    int i = group_id;
    return read_data_group_id(i);
}

std::string Node::read_data_group_id(int& group_id){
    // File buffer to read the file from
    std::filebuf fb;
    
    fb.open(filepath, std::ios::in);

    // Check that the file successfully opened
    if(!fb.is_open()){
        return "";
        // throw std::invalid_argument("Unable to open file");
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

    for(int i = 0; i < 2; i++){
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
    // If data is already loaded, do not reload
    if(data_loaded) return;

    std::string data_str = read_data_group(Node::data);

    // Initialize the map
    node_contents = new std::unordered_map<std::string, std::string>();
    // Don't try to load if there is no data
    if(data_str.length() == 0){
        data_loaded = true;
        return;
    }



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

// Writing is based on the format found on lines 11-18 of Node.h
void Node::write_data(){
    // No need to write data if data hasn't changed
    if(!data_changed) return;

    if(!data_loaded) load_data();

    std::ofstream ofs(filepath, std::ios::trunc);

    // Group 0 Writing
    ofs << std::to_string(id) << char(A_RECORD_SEP) << name;
    ofs << char(A_GROUP_SEP);

    // Group 2 Writing
    if(node_contents->size() == 0){
        data_changed = false;
        return;
    }
    auto last_elm = node_contents->end();
    for(auto it = node_contents->begin(); it != last_elm; ++it){
        ofs << it->first << char(A_UNIT_SEP) << it->second << char(A_RECORD_SEP);
    }

    data_changed = false;
}

void Node::dump_data(){
    if(!data_loaded) return;
    
    write_data();
    if(node_contents != NULL) delete node_contents;
    data_loaded = false;
}

std::string& Node::get_value(std::string key){
    if(!data_loaded) load_data();

    return node_contents->at(key);
}

void Node::set_value(std::string key, std::string value){
    if(!data_loaded) load_data();

    node_contents->erase(key);
    node_contents->emplace(key, value);
    data_changed = true;
}


void Node::erase_value(std::string key){
    if(!data_loaded) load_data();

    node_contents->erase(key);
    data_changed = true;
}

bool Node::delete_node(){
    return remove(filepath.c_str()) == 0;
}

Node::~Node(){
    dump_data();
}
