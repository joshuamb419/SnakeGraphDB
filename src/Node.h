#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include "NodeData.h"
class Node{
    private:
        // id the node will be refered to by
        int id;

        // text name used for searching + readability
        std::string name;

        // number of nodes that point to this node
        int referenceCount;

        // Nodes this node points to
        std::vector<int> connectionIds;

        // Data stored in this node + file management
        NodeData* nodeData;
    public:
        // Load node from file
        Node(std::string& folder, int& id);
        // Create Node with id and name
        Node(std::string& folder, int& id, std::string& name);

        // ID of this node
        int& getId();

        // Name of this node
        std::string& getName();

        // Get ids of the connected nodes
        std::vector<int>& getConnectionIds();

        // Increments the reference counter
        void addReference();
        // Get count of nodes pointing to this node
        int& getReferenceCount();
        // Decrements the reference counter, if reference counter becomes zero the node self-destructs
        void removeReference();

        // Adds connection to target node
        void addConnection(int& targetId);
        // Removes connection from this node to the target
        void removeConnection(int& targetId);
        

        // Delete nodedata
        ~Node();
};
#endif