#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include "SGNode.h"

int main(int argc, char** argv){
    std::cout << "Input a number: ";
    int input;
    std::cin >> input;

    std::string folder = "./db_folder";
    std::string name = "test_muffins";

    std::printf("Writing node 1...\n");
    SGNode *write_node = new SGNode(folder, 1, name, true);
    write_node->setValue("Ingredient Count", 32);
    write_node->setValue("Has Gluten", true);
    write_node->setValue("Cups of Flour", 3.75);
    write_node->setValue("Doubling Recomendations", "Dont give bradley sugar");
    write_node->writeData();
    delete write_node;
    std::printf("Node 1 written\n");

    std::printf("Reading node 1...\n");
    SGNode *read_node = new SGNode(folder, 1, name);
    std::printf("Ingerdient Count: %f\n", read_node->getValue(""))


}
