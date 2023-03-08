#include "SGNode.h"
#include "AsciiControlCodes.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <queue>
#include <sstream>

SGNode::SGNode(std::string& folder, int id, std::string& name) : SGNode(folder, id, name, false) {}

SGNode::SGNode(std::string& folder, int id, std::string& name, bool overwrite){
    this->id = id;
    this->name = name;
    this->filepath = folder + std::to_string(id) + FILE_EXT;
    
    if(overwrite) {
        data_changed = true;
        data_loaded = true;
        node_contents = new std::unordered_map<std::string, std::vector<unsigned char>>();
        deleteNode();
        writeData();
    }
}

int& SGNode::getId(){
    return id;
}

std::string& SGNode::getName(){
    return name;
}

bool& SGNode::isLoaded(){
    return data_loaded;
}

void SGNode::loadData(){
    // If data is already loaded, do not reload
    if(data_loaded) return;

    node_contents = new std::unordered_map<std::string, std::vector<unsigned char>>();

    std::ifstream ifs(filepath, std::ios::binary);

    if(!ifs.is_open()) throw std::invalid_argument("Unable to open file");

    data_loaded = true;

    char c = ' ';
    while(c != A_GROUP_SEP) { ifs.get(c); }

    ifs.get(c);

    while(c != char(A_GROUP_SEP)) {
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

        // std::printf("\nKey: %s\nPosition: %d\nLength: %d\nValue: ", key.c_str(), pos, len);
        // for(char d : value) {
        //     std::cout << d;
        // }
        // std::cout << std::endl;

        ifs.get(c);
    }
}

// Writing is based on the format found on lines 11-18 of SGNode.h
void SGNode::writeData(){

    // No need to write data if data hasn't changed
    if(!data_changed) return;
    // If something has changed but the data isnt loaded we need it in memory
    if(!data_loaded) loadData();

    std::ofstream ofs(filepath, std::ios::trunc | std::ios::binary);

    // Group 0 Writing
    ofs << std::to_string(id) << char(A_RECORD_SEP) << name;
    ofs << char(A_GROUP_SEP);

    // Group 1 Writing
    for(std::string str : labels) {
        ofs << str;

    }

    // Group 2 Writing
    uint32_t pos = ofs.tellp();

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

    pos++;

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
    }

    ofs << char(A_GROUP_SEP);

    // Group 3 writing

    std::vector<unsigned char> value = std::vector<unsigned char>();
    while(!key_order.empty()) {
        value = node_contents->at(key_order.front());
        key_order.pop();
        ofs.write((char*) value.data(), value.size());
    }

    ofs.close();

    data_changed = false;
}

void SGNode::dumpData(){  
    if(!data_loaded) return;
    writeData();
    delete node_contents;
    data_loaded = false;
}

std::vector<unsigned char>& SGNode::getRawData(std::string key) {
    if(!data_loaded) loadData();

    return node_contents->at(key);
}

int SGNode::getByteArray(std::string key, char*& pointer) {
    std::vector<unsigned char>& data = getRawData(key);
    pointer = (char *) data.data();
    return data.size();
}

std::string& SGNode::getString(std::string key) {
    std::vector<unsigned char>& data = getRawData(key);
    std::string string = std::string(data.begin(), data.end());
    return string;
}

int32_t& SGNode::getInt32(std::string key) {
    std::vector<unsigned char>& data = getRawData(key);
    return *((int32_t*) data.data());
}

double& SGNode::getDouble(std::string key) {
    std::vector<unsigned char>& data = getRawData(key);
    return *((double*) data.data());
}

bool& SGNode::getBool(std::string key) {
    std::vector<unsigned char>& data = getRawData(key);
    return *((bool*) data.data());
}

void SGNode::setValue(std::string key, std::vector<unsigned char> value) {
    if(!data_loaded) loadData();

    node_contents->erase(key);
    node_contents->emplace(key, value);
    data_changed = true;
}

void SGNode::setValue(std::string key, char* pointer, int length) {
    std::vector<unsigned char> data(pointer, pointer + length);
    setValue(key, data);
}

void SGNode::setValue(std::string key, std::string value) {
    std::vector<unsigned char> data(value.begin(), value.end());
    setValue(key, data);
}

void SGNode::setValue(std::string key, int32_t value) {
    unsigned char* d = reinterpret_cast<unsigned char*>(&value);
    std::vector<unsigned char> data(d, d + sizeof(value));
    setValue(key, data);
}

void SGNode::setValue(std::string key, double value) {
    unsigned char* d = reinterpret_cast<unsigned char*>(&value);
    std::vector<unsigned char> data(d, d + sizeof(value));
    setValue(key, data);
}

void SGNode::setValue(std::string key, bool value) {
    unsigned char* d = reinterpret_cast<unsigned char*>(&value);
    std::vector<unsigned char> data(d, d + sizeof(value));
    setValue(key, data);
}


void SGNode::eraseValue(std::string key) {
    if(!data_loaded) loadData();

    node_contents->erase(key);
    data_changed = true;
}

bool SGNode::deleteNode() {
    return remove(filepath.c_str()) == 0;
}

SGNode::~SGNode() {
    dumpData();
}
