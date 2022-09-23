#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "NodeData.h"
class Node{
    private:
        // ID the node will be refered to by
        int id;

        // Text name used for searching + readability
        std::string name;

        // number of nodes that point to this node
        int reference_count;

        // Nodes this node points to
        std::vector<int> connection_ids;

        // Data stored in this node + file management
        NodeData* node_data;
    public:
        // Load node from file
        Node(std::string& folder, int& id);
        // Create Node with id and name
        Node(std::string& folder, int& id, std::string& name);

        // ID of this node
        int& get_id();

        // Name of this node
        std::string& get_name();

        // Get ids of the connected nodes
        std::vector<int>& get_connection_ids();

        // Increments the reference counter
        void add_reference();
        // Get count of nodes pointing to this node
        int& get_reference_count();
        // Decrements the reference counter, if reference counter becomes zero the node self-destructs
        void remove_reference();

        // Adds connection to target node
        void add_connection(int& targetId);
        // Removes connection from this node to the target
        void remove_connection(int& targetId);
        

        // Delete nodedata
        ~Node();
};
#endif