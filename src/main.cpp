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
    Node node(folder, id, name);
    node.load_data();
}