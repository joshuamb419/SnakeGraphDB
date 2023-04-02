#include <cstdio>
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include "Node.h"

int main(int argc, char** argv){
    std::string folder = "./db_folder/";
    std::string name = "test_muffins";

    std::printf("Writing node 1...\n");
    Node *write_node = new Node(folder, 1, name, true);
    write_node->setInt32("Ingredient Count", 32);
    write_node->setBool("Has Gluten", true);
    write_node->setDouble("Cups of Flour", 3.75);
    write_node->setString("Doubling Recomendations", std::string("Dont give bradley sugar"));
    write_node->writeData();
    delete write_node;
    std::printf("\nNode 1 written\n");

    std::printf("\nReading node 1...\n\n");
    Node *read_node = new Node(folder, 1, name);
    std::printf("Ingerdient Count: %d\n", read_node->getInt32("Ingredient Count"));
    std::printf("Has Gluten: %d\n", read_node->getBool("Has Gluten"));
    std::printf("Cups of Flour: %f\n", read_node->getDouble("Cups of Flour"));
    std::printf("Doubling Recomendations: %s\n", read_node->getString("Doubling Recomendations").c_str());
    delete read_node;
    std::printf("\n\nFinished Reading Node 1\n");
}
