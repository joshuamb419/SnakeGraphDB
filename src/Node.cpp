#include "Node.h"
#include "AsciiControlCodes.h"
#include <string.h>
#include <algorithm>

Node::Node(std::string& folder, int& id){
    this->id = id;
    this->node_data = new NodeData(folder, id);

    // Pull the name from the file
    std::string idGroup = node_data->read_data_group(NodeData::identification);
    this->name = idGroup.substr(idGroup.find(A_RECORD_SEP) + 1, idGroup.length());

    // Find out how many nodes reference this node
    std::string connectionGroup = node_data->read_data_group(NodeData::connections);
    int connectionSplit = connectionGroup.find(A_RECORD_SEP);
    this->reference_count = atoi(connectionGroup.substr(0, connectionSplit).c_str());

    // Get a list of the nodes this node references
    std::string connectionListString = connectionGroup.substr(connectionSplit + 1, connectionGroup.length());
    int idStart = 0;
    for(int i = 0; i < connectionListString.length(); i++){
        if(connectionListString.at(i) == A_UNIT_SEP){
            connection_ids.push_back(atoi(connectionListString.substr(idStart, i).c_str()));
            idStart = ++i;
        }
    }
}

Node::Node(std::string& folder, int& id, std::string& name){
    this->id = id;
    this->name = name;
    this->reference_count = 0;
    this->node_data = new NodeData(folder, id, name);
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

Node::~Node(){
    delete node_data;
}
