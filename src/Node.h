#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <unordered_map>

class Node{

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
        std::string FILE_EXT = ".sgnd";

        // path to this nodes file
        std::string filepath;

        // non-control data stored in the node
        std::unordered_map<std::string, std::vector<unsigned char>>* node_contents = NULL;

        bool data_loaded = false;

        bool data_changed = false;

    public:
        // Load node from file
        Node(std::string& folder, int id);
        // Create Node with id and name
        Node(std::string& folder, int id, std::string& name);
        // Says whether or not to overwrite an existing node with the same id
        Node(std::string& folder, int id, std::string& name, bool overwrite);

        // ID of this node
        int& get_id();

        // Name of this node
        std::string& get_name();

        // Returns if the data is currently loaded
        bool& is_loaded();
        // Loads data from file into memory
        // If data is already loaded it will NOT reload data
        void load_data();
        // Writes data to disk
        void write_data();
        // Dumps data out of memory, calls write_data first if changes
        void dump_data();
        
        // Returns the value of the key, null if key is not in map
        // if data is not loaded this method will cause data to be loaded
        std::vector<unsigned char>& get_value(std::string key);
        // Sets the value of a key
        // if data is not loaded this method will cause data to be loaded
        void set_value(std::string key, std::vector<unsigned char> value);
        // Erases the key and the value associated with that key
        // if data is not loaded this method will cause data to be loaded
        void erase_value(std::string key);

        // Deletes the file this node references, the object will not be lost
        // Returns false if the node failed to delete
        bool delete_node();

        // Delete unordered map, will write data first if there are changes
        ~Node();
};
#endif