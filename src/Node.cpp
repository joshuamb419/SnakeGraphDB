#include "Node.h"
#include "AsciiControlCodes.h"
#include "Label.h"
#include <filesystem>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <queue>
#include <sstream>

using namespace SnakeGraph;

bool Node::nodeFileExists(std::string &folder, int id) {
    std::string filepath = folder + std::to_string(id) + FILE_EXT;
    return std::filesystem::exists(filepath);
}

Node::Node(std::string& folder, int id) {
    // If node does not exist throw an error
    if(!nodeFileExists(folder, id)) {
        throw new std::invalid_argument("Node " + std::to_string(id)
                                        + " does not exist in database \'" + folder + "\'");
    }

    this->id = id;
    this->filepath = folder + std::to_string(id) + FILE_EXT;

    loadName();
    loadLabels();
}

Node::Node(std::string& folder, int id, std::string& name) : Node(folder, id, name, false) {}

Node::Node(std::string& folder, int id, std::string& name, bool overwrite){
    this->id = id;
    this->name = name;
    this->filepath = folder + std::to_string(id) + FILE_EXT;

    // Only write if we are supposed to be overwritting or the file does not exist
    if(overwrite || !nodeFileExists(folder, id)) {
        dataChanged = true;
        dataLoaded = true;
        nodeContents = new std::unordered_map<std::string, std::vector<unsigned char>>();
        deleteNode();
        writeData();
    } else {
        throw new std::invalid_argument("Node " + std::to_string(id)
                                        + " already exists in database \'" + folder + "\'");
    }
}

void Node::loadName() {
    std::ifstream ifs(filepath, std::ios::binary);

    // Increment ifs to start of node name
    char c = ' ';
    while(c != A_RECORD_SEP){ ifs.get(c); }

    // Read name
    std::vector<char> nameVector;
    do {
        nameVector.push_back(c);
        ifs.get(c);
    } while(c != A_GROUP_SEP);

    ifs.close();

    this->name = std::string(nameVector.begin(), nameVector.end());
}

void Node::loadLabels() {
    std::ifstream ifs(filepath, std::ios::binary);

    // Skip groups 0 and 1 (identification and labels)
    char c = ' ';
    while(c != A_GROUP_SEP) { ifs.get(c); }
    ifs.get(c);
}

void Node::writeLabels() {
    writeData();
}

int& Node::getId(){
    return id;
}

std::string& Node::getName(){
    return name;
}

bool& Node::isLoaded(){
    return dataLoaded;
}

void Node::loadData(){
    // If data is already loaded, do not reload
    if(dataLoaded) return;

    nodeContents = new std::unordered_map<std::string, std::vector<unsigned char>>();

    std::ifstream ifs(filepath, std::ios::binary);

    if(!ifs.is_open()) throw std::invalid_argument("Unable to open file");

    dataLoaded = true;

    // Skip groups 0 and 1 (identification and labels)
    char c = ' ';
    while(c != A_GROUP_SEP) { ifs.get(c); }
    ifs.get(c);
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

        nodeContents->emplace(key, value);

//         std::printf("\nKey: %s\nPosition: %d\nLength: %d\nValue: ", key.c_str(), pos, len);
//         for(char d : value) {
//             std::cout << d;
//         }
//         std::cout << std::endl;

        ifs.get(c);
    }
}

// Writing is based on the format found on lines 11-18 of SGNode.h
void Node::writeData(){

    // No need to write data if data hasn't changed
    if(!dataChanged) return;
    // If something has changed but the data isnt loaded we need it in memory
    if(!dataLoaded) loadData();

    std::ofstream ofs(filepath, std::ios::trunc | std::ios::binary);

    // Group 0 Writing
    ofs << std::to_string(id) << char(A_RECORD_SEP) << name;
    ofs << char(A_GROUP_SEP);

    // Group 1 Writing
    // For every label write the encoded version of it to disk
    for(Label* label : labels) {
        for(unsigned char uc : label->encodeLabel()) {
            ofs << uc;
        }
        ofs << char(A_UNIT_SEP);
    }
    ofs << char(A_GROUP_SEP);

    // Group 2 Writing
    // Find position keys group begins at
    uint32_t pos = ofs.tellp();

    // Do nothing if there is no data
    if(nodeContents->size() == 0) {
        ofs << char(A_GROUP_SEP);
        ofs.close();
        dataChanged = false;
        return;
    }

    // Figure out where group 3 will begin
    for(auto it : *nodeContents) {
        pos += it.first.length();
        pos += 9;
    }

    pos++;

    // Push keys into queue in the order they are written to ensure
    // data is written in the same sorger
    std::queue<std::string> key_order = std::queue<std::string>();

    int i = 0;
    for(auto it : *nodeContents) {
        key_order.push(it.first);
        // Write key
        ofs << it.first;
        ofs << char(A_UNIT_SEP);
        uint32_t size = it.second.size();
        // Write position of data
        ofs.write((char*) (&pos), 4);
        // Write len of data
        ofs.write((char*) (&size), 4);
        // increment pos counter by size of data
        pos += size;
    }

    ofs << char(A_GROUP_SEP);

    // Group 3 writing

    // Write data in the same order as the keys so position values work as expected
    std::vector<unsigned char> value;
    while(!key_order.empty()) {
        value = nodeContents->at(key_order.front());
        key_order.pop();
        ofs.write((char*) value.data(), value.size());
    }

    ofs.close();

    dataChanged = false;
}

void Node::dumpData(){
    if(!dataLoaded) return;
    writeData();
    delete nodeContents;
    dataLoaded = false;
}

std::vector<unsigned char>& Node::getRawData(std::string key) {
    if(!dataLoaded) loadData();

    return nodeContents->at(key);
}

int Node::getByteArray(std::string key, char*& pointer) {
    std::vector<unsigned char>& data = getRawData(key);
    pointer = (char *) data.data();
    return data.size();
}

std::string Node::getString(std::string key) {
    std::vector<unsigned char>& data = getRawData(key);
    return std::string(data.begin(), data.end());
}

int32_t& Node::getInt32(std::string key) {
    std::vector<unsigned char>& data = getRawData(key);
    return *((int32_t*) data.data());
}

double& Node::getDouble(std::string key) {
    std::vector<unsigned char>& data = getRawData(key);
    return *((double*) data.data());
}

bool& Node::getBool(std::string key) {
    std::vector<unsigned char>& data = getRawData(key);
    return *((bool*) data.data());
}

void Node::setRawValue(std::string key, std::vector<unsigned char> value) {
    if(!dataLoaded) loadData();

    nodeContents->erase(key);
    nodeContents->emplace(key, value);
    dataChanged = true;
}

void Node::setByteArray(std::string key, char* pointer, int length) {
    std::vector<unsigned char> data(pointer, pointer + length);
    setRawValue(key, data);
}

void Node::setString(std::string key, std::string value) {
    std::vector<unsigned char> data(value.begin(), value.end());
    setRawValue(key, data);
}

void Node::setInt32(std::string key, int32_t value) {
    unsigned char* d = reinterpret_cast<unsigned char*>(&value);
    std::vector<unsigned char> data(d, d + sizeof(value));
    setRawValue(key, data);
}

void Node::setDouble(std::string key, double value) {
    unsigned char* d = reinterpret_cast<unsigned char*>(&value);
    std::vector<unsigned char> data(d, d + sizeof(value));
    setRawValue(key, data);
}

void Node::setBool(std::string key, bool value) {
    unsigned char* d = reinterpret_cast<unsigned char*>(&value);
    std::vector<unsigned char> data(d, d + sizeof(value));
    setRawValue(key, data);
}


void Node::eraseValue(std::string key) {
    if(!dataLoaded) loadData();

    nodeContents->erase(key);
    dataChanged = true;
}

bool Node::deleteNode() {
    return remove(filepath.c_str()) == 0;
}

Node::~Node() {
    dumpData();
}
