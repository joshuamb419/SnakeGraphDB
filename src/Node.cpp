#include "Node.h"
#include <string>

Node::Node(std::string id){
    this->id = id;
}

std::string Node::get_id(){
    return id;
}

Node** Node::get_connected_nodes(){
    return connected_nodes;
}