#ifndef SG_NODE_H
#define SG_NODE_H

#include <string>
#include <vector>
#include <unordered_map>

class SGNode{

    /*
    * *.sgnd files are formatted as follows
    * Groups are seperated by ascii group seperator characters
    * Usages of ':' represent usages of the ascii record seperator
    * Usages of ',' represent usages of the ascii unit seperator
    * 
    * Group 0 (identification)  <node_id>:<node_name>
    * Group 1 (keys)            <key_1>,<32_bit_pos_1><32_bit_len_1><key_2>,<32_bit_pos_2><32_bit_len_2>...<key_n>,<32_bit_pos_n><32_bit_len_n>
    * Group 2 (data)            <value_1><value_2>...<value_n>
    */

    private:
        // ID the node will be refered to by
        int id;

        // Text name used for searching + readability
        std::string name;

        // string to be appended after node id to get file name
        std::string FILE_EXT = ".sgn";

        // path to this nodes file
        std::string filepath;

        // non-control data stored in the node
        std::unordered_map<std::string, std::vector<unsigned char>>* node_contents = NULL;

        bool data_loaded = false;

        bool data_changed = false;

    protected:
        SGNode();
        // Load SGNode
        SGNode(std::string& folder, int id, std::string& name);
        // Says whether or not to overwrite an existing node with the same id
        SGNode(std::string& folder, int id, std::string& name, bool writeOnLoad);

        // Deletes the file this node references, the object will not be lost
        // Returns false if the node failed to delete
        bool deleteNode();

        // Delete unordered map, will write data first if there are changes
        ~SGNode();
    public:
        // ID of this node
        int& getId();

        // Name of this node
        std::string& getName();

        // Returns if the data is currently loaded
        bool& isLoaded();
        // Loads data from file into memory
        // If data is already loaded it will NOT reload data
        void loadData();
        // Writes data to disk
        void writeData();
        // Dumps data out of memory, calls write_data first if changes
        void dumpData();
        
        // Returns the value of the key, null if key is not in map
        // if data is not loaded this method will cause data to be loaded
        std::vector<unsigned char>& getValue(std::string key);
        // returns the length of the value
        int getValue(std::string key, char*& pointer);
        void getValue(std::string key, std::string& value);
        void getValue(std::string key, int32_t& value);
        void getValue(std::string key, double& value);
        void getValue(std::string key, bool& value);

        // Sets the value of a key
        // if data is not loaded this method will cause data to be loaded
        void setValue(std::string key, std::vector<unsigned char> value);
        void setValue(std::string key, char* pointer, int length);
        void setValue(std::string key, std::string value);
        void setValue(std::string key, int32_t value);
        void setValue(std::string key, double value);
        void setValue(std::string key, bool value);
        // Erases the key and the value associated with that key
        // if data is not loaded this method will cause data to be loaded
        void eraseValue(std::string key);

    friend class SnakeGraph;
    friend class SGLinkManager;
};
#endif