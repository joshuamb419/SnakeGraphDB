#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
class Node{
    private:
        std::vector<std::string>* connected_node_ids;
        std::vector<Node*> connected_nodes;
        std::string id;
    public:
        // Create Node with id
        Node(std::string id);
        // Create Node from file
        Node(std::ifstream &input_file);

        // ID of this node
        std::string& get_id();
        // Vector of nodes connected to this one
        std::vector<Node*>& get_connected_nodes();
};
#endif