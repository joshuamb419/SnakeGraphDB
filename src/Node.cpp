#include "Node.h"
#include "AsciiControlCodes.h"
#include <string.h>
#include <algorithm>

Node::Node(std::string& folder, int& id){
    this->id = id;
    this->nodeData = new NodeData(folder, &(this->id));

    // Pull the name from the file
    std::string idGroup = nodeData->getIdGroup();
    this->name = idGroup.substr(idGroup.find(A_RECORD_SEP) + 1, idGroup.length());

    // Find out how many nodes reference this node
    std::string connectionGroup = nodeData->getConnectionsGroup();
    int connectionSplit = connectionGroup.find(A_RECORD_SEP);
    this->referenceCount = atoi(connectionGroup.substr(0, connectionSplit).c_str());

    // Get a list of the nodes this node references
    std::string connectionListString = connectionGroup.substr(connectionSplit + 1, connectionGroup.length());
    int idStart = 0;
    for(int i = 0; i < connectionListString.length(); i++){
        if(connectionListString.at(i) == A_UNIT_SEP){
            connectionIds.push_back(atoi(connectionListString.substr(idStart, i).c_str()));
            idStart = ++i;
        }
    }
}

Node::Node(std::string& folder, int& id, std::string& name){
    this->id = id;
    this->name = name;
    this->referenceCount = 0;
    this->nodeData = new NodeData(folder, &(this->id), name);
}

int& Node::getId(){
    return id;
}

std::string& Node::getName(){
    return name;
}

std::vector<int>& Node::getConnectionIds(){
    return connectionIds;
}

void Node::addReference(){
    referenceCount++;
}

int& Node::getReferenceCount(){
    return referenceCount;
}

void Node::removeReference(){
    if(--referenceCount == 0){
        delete this;
    }
}

void Node::addConnection(int& targetId){
    if(std::find(connectionIds.begin(), connectionIds.end(), targetId) == connectionIds.end()){
        connectionIds.push_back(targetId);
    }
}

void Node::removeConnection(int& targetId){
    auto it = std::find(connectionIds.begin(), connectionIds.end(), targetId);
    if(it != connectionIds.end()){
        connectionIds.erase(it);
    }
}

Node::~Node(){
    delete nodeData;
}
