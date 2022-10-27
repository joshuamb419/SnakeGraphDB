#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "Node.h"
#include "AsciiControlCodes.h"

int main(int argc, char** argv){
    std::string folder = "./db_folder/";
    int id = 1;
    Node *node = new Node(folder, id);
    std::printf("Recipie for: %s\nIngredients:\n%s\nDirections: %s\n",
                node->get_name().c_str(),
                node->get_value("ingredients").c_str(),
                node->get_value("recipie").c_str());
    delete node;
}