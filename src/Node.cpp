#include "Node.h"
#include "AsciiControlCodes.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <queue>

Node::Node(std::string& folder, int id){
    this->id = id;
    this->filepath = folder + std::to_string(id) + FILE_EXT;

    // Pull the name from the file
    std::string idGroup = read_data_group(Node::identification);
    this->name = idGroup.substr(idGroup.find(A_RECORD_SEP) + 1);
}

Node::Node(std::string& folder, int id, std::string& name){
    Node(folder, id, name, true);
}

Node::Node(std::string& folder, int id, std::string& name, bool overwrite){
    this->id = id;
    this->name = name;
    this->filepath = folder + std::to_string(id) + FILE_EXT;
    data_changed = true;
    if(overwrite) {
        data_loaded = true;
        node_contents = new std::unordered_map<std::string, std::vector<unsigned char>>();
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
    int size = 10;
    int buffer_used = 0;
    char* buffer = (char*) malloc(size);

    // Read the selected group to buffer
    while(peek != EOF && peek != A_GROUP_SEP){
         // If more buffer is full, expand the buffer
        if(buffer_used + 1 >= size){
            size *= 2;
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

bool& Node::is_loaded(){
    return data_loaded;
}

void Node::load_data(){
    // If data is already loaded, do not reload
    if(data_loaded) return;

    node_contents = new std::unordered_map<std::string, std::vector<unsigned char>>();

    data_loaded = true;
}

// Writing is based on the format found on lines 11-18 of Node.h
void Node::write_data(){
    std::cout << this->id << std::endl;

    // No need to write data if data hasn't changed
    if(!data_changed) return;
    // If something has changed but the data isnt loaded we need it in memory
    if(!data_loaded) load_data();

    std::ofstream ofs(filepath, std::ios::trunc | std::ios::binary);

    // Group 0 Writing
    ofs << std::to_string(id) << char(A_RECORD_SEP) << name;
    ofs << char(A_GROUP_SEP);

    uint32_t pos = ofs.tellp();
    pos--;

    // Group 1 Writing

    // Do nothing if there is no data
    if(node_contents->size() == 0) {
        ofs << char(A_GROUP_SEP);
        ofs.close();
        data_changed = false;
        return;
    }

    for(auto it : *node_contents) {
        pos += it.first.length();
        pos += 9;
    }

    std::queue<std::string> key_order = std::queue<std::string>();

    int i = 0;
    for(auto it : *node_contents) {
        ofs << it.first;
        key_order.push(it.first);
        uint32_t size = it.second.size();
        ofs.write((char*) (&pos), 4);
        ofs.write((char*) (&size), 4);
        pos += size;

        if(++i != node_contents->size()) ofs << char(A_RECORD_SEP);
    }

    ofs << char(A_GROUP_SEP);

    // Group 2 writing

    std::vector<unsigned char> value = std::vector<unsigned char>();
    while(!key_order.empty()) {
        value = node_contents->at(key_order.front());
        key_order.pop();
        ofs.write((char*) value.data(), value.size());
    }

    ofs.close();

    data_changed = false;
}

void Node::dump_data(){  
    write_data();
    if(node_contents != NULL) delete node_contents;
    data_loaded = false;
}

std::vector<unsigned char>& Node::get_value(std::string key) {
    if(!data_loaded) load_data();

    return node_contents->at(key);
}

void Node::set_value(std::string key, std::vector<unsigned char> value){
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
    // dump_data();
}
