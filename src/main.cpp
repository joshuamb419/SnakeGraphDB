#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Node.h"
#include "AsciiControlCodes.h"

int main(int argc, char** argv){
    std::string folder = "/mnt/c/Users/Joshua/Downloads/testFolder/";
    std::string name = "test";
    int id = 1;
    Node node(folder, id);
    node.add_reference();
    node.add_reference();
    // node.set_value("key1", "one");
    // node.set_value("key2", "two");
    std::cout << node.get_name() << std::endl << node.get_reference_count();
    node.write_data();
}