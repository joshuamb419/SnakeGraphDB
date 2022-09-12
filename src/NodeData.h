#ifndef NODE_DATA_H
#define NODE_DATA_h

#include <string>
#include <unordered_map>


/*
* *.sgnd files are formatted as follows
* Groups are seperated by ascii group seperator characters
* Usages of ':' represent usages of the ascii record seperator
* Usages of ',' represent usages of the ascii unit seperator
* 
* Group 1 (identification)    <node_id>:<node_name>
* Group 2 (connections)       <referenceCount>:<connectionIds>
* Group 3 (data)              <key_1>,<value_1>:<key_2>,<value_2>:...:<key_e>,<value_e>
*/

class NodeData{
    private:
        // string to be appended after node id to get file name
        std::string FILE_EXT = "_data.sgnd";

        // path to this nodes file
        std::string filepath;

        // non-control data stored in the node
        std::unordered_map<std::string, std::string>* nodeContents;

        bool dataLoaded = false;

        // Returns only the requested data group
        std::string& readDataGroup(int group_id);
        // Returns an array split into the three data groups
        std::string* readDataGroups();
    public:
        // Load node from the provided folder with
        NodeData(std::string& folder, int* id);
        // Create node in folder with provided id and name
        NodeData(std::string& folder, int* id, std::string& name);

        // Returns if the data is currently loaded
        bool& isLoaded();
        // Loads data from file into memory
        void loadData();
        // Writes data to disk
        void writeData();
        // Dumps data out of memory, calls writeData first
        void dumpData();

        // Returns the Identification group
        std::string& getIdGroup();
        // Returns the Connections group
        std::string& getConnectionsGroup();
        // Returns the Data group();
        std::string& getDataGroup();

        // Returns the value of the key, null if key is not in map
        // if data is not loaded this method will cause data to be loaded
        std::string& getValue(std::string key);
        // Sets the value of a key, return indicates if the operation was successful
        // if data is not loaded this method will cause data to be loaded
        bool setValue(std::string key, std::string value);
        // Erases the key and the value associated with that key, return indicates if the operation was successful
        // if data is not loaded this method will cause data to be loaded
        bool eraseValue(std::string key);

        // Writes data to disk and frees up memory
        ~NodeData();
};

#endif