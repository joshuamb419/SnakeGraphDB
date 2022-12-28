#include "Node.h"
#include "AsciiControlCodes.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <queue>
#include <sstream>

Node::Node(std::string& folder, int id){
    this->id = id;
    this->filepath = folder + std::to_string(id) + FILE_EXT;

    // Pull the name from the file
    std::ifstream ifs(filepath, std::ios::binary);
    if(!ifs.is_open()) throw std::invalid_argument("Unable to open file");

    char c;
    ifs.get(c);
    while(c != A_RECORD_SEP) {
        ifs.get(c);
    }

    ifs.get(c);
    while(c != A_GROUP_SEP) {
        this->name += c;
        ifs.get(c);
    }
}

Node::Node(std::string& folder, int id, std::string& name) : Node(folder, id, name, true) {}

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

bool& Node::is_loaded(){
    return data_loaded;
}

void Node::load_data(){
    // If data is already loaded, do not reload
    if(data_loaded) return;

    node_contents = new std::unordered_map<std::string, std::vector<unsigned char>>();

    std::ifstream ifs(filepath, std::ios::binary);

    if(!ifs.is_open()) throw std::invalid_argument("Unable to open file");

    data_loaded = true;

    char c = ' ';
    while(c != A_GROUP_SEP) { ifs.get(c); }

    ifs.get(c);

    while(c != A_GROUP_SEP) {
        std::string key;
        while(c != A_UNIT_SEP) {
            key += c;
            ifs.get(c);
        }

        uint32_t pos;
        ifs.read((char*) (&pos), sizeof(pos));

        uint32_t len;
        ifs.read((char*) (&len), sizeof(len));

        int old_pos = ifs.tellg();
        ifs.seekg(pos);
        std::vector<unsigned char> value;
        for(int i = 0; i < len; i++) {
            ifs.get(c);
            value.push_back(c);
        }
        ifs.seekg(old_pos);

        node_contents->emplace(key, value);

        std::printf("\nKey: %s\nPosition: %d\nLength: %d\nValue: ", key.c_str(), pos, len);
        for(char d : value) {
            std::cout << d;
        }
        std::cout << std::endl;

        ifs.get(c);
    }

}

// Writing is based on the format found on lines 11-18 of Node.h
void Node::write_data(){

    // No need to write data if data hasn't changed
    if(!data_changed) return;
    // If something has changed but the data isnt loaded we need it in memory
    if(!data_loaded) load_data();

    std::ofstream ofs(filepath, std::ios::trunc | std::ios::binary);

    // Group 0 Writing
    ofs << std::to_string(id) << char(A_RECORD_SEP) << name;
    ofs << char(A_GROUP_SEP);

    uint32_t pos = ofs.tellp();

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
        pos += 10;
    }

    std::queue<std::string> key_order = std::queue<std::string>();

    int i = 0;
    for(auto it : *node_contents) {
        ofs << it.first;
        ofs << char(A_UNIT_SEP);
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
    dump_data();
}
