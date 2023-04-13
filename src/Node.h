#ifndef SG_NODE_H
#define SG_NODE_H

#include <string>
#include <vector>
#include <set>
#include <unordered_map>

#include "Label.h"

namespace SnakeGraph {

    class Node{
        /*
          * *.sgnd files are formatted as follows
          * Groups are seperated by ascii group seperator characters
          * Usages of ':' represent usages of the ascii record seperator
          * Usages of ',' represent usages of the ascii unit seperator
          *
          * Group 0 (identification)  <node_id>:<node_name>
          * Group 1 (labels)          <label_1>,<label_2>...<label_n>
          * Group 2 (keys)            <key_1>,<32_bit_pos_1><32_bit_len_1><key_2>,<32_bit_pos_2><32_bit_len_2>...<key_n>,<32_bit_pos_n><32_bit_len_n>
          * Group 3 (data)            <value_1><value_2>...<value_n>
          */

        private:
            // ID the node will be refered to by
            int id;

            // Text name used for searching + readability
            std::string name;

            // string to be appended after node id to get file name
            constexpr static const std::string FILE_EXT = ".sgn";

            // path to this nodes file
            std::string filepath;

            // non-control data stored in the node
            std::unordered_map<std::string, std::vector<unsigned char>>* nodeContents = NULL;

            // Labels used for querying
            std::set<Label*> labels;

            bool dataLoaded = false;

            bool dataChanged = false;

            static bool nodeFileExists(std::string& folder, int id);

            void loadName();

            void loadLabels();

            void writeLabels();

        public:
            // Load Existing Node
            Node(std::string& folder, int id);
            // Create New Node
            Node(std::string& folder, int id, std::string& name);
            // Says whether or not to overwrite an existing node with the same id
            Node(std::string& folder, int id, std::string& name, bool overwrite);

            // Deletes the file this node references, the object will not be lost
            // Returns false if the node failed to delete
            bool deleteNode();

            // Delete unordered map, will write data first if there are changes
            ~Node();
            //public:
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
            std::vector<unsigned char>& getRawData(std::string key);
            // returns the length of the value
            int getByteArray(std::string key, char*& pointer);
            std::string getString(std::string key);
            int32_t& getInt32(std::string key);
            double& getDouble(std::string key);
            bool& getBool(std::string key);

            // Sets the value of a key
            // if data is not loaded this method will cause data to be loaded
            void setRawValue(std::string key, std::vector<unsigned char> value);
            void setByteArray(std::string key, char* pointer, int length);
            void setString(std::string key, std::string value);
            void setInt32(std::string key, int32_t value);
            void setDouble(std::string key, double value);
            void setBool(std::string key, bool value);
            // Erases the key and the value associated with that key
            // if data is not loaded this method will cause data to be loaded
            void eraseValue(std::string key);

            friend class SnakeGraph;
            friend class LinkManager;
    };
}
#endif