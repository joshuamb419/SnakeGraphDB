#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "Node.h"

int main(int argc, char** argv){
    std::string folder = "./db_folder/";

    std::string name = "TestNode";

    Node writeNode(folder, 1, name);

    writeNode.set_value("one", std::string("one"));
    writeNode.set_value("two", true);
    writeNode.set_value("three", 32);
    writeNode.set_value("four", 32.5);
    writeNode.dump_data();

    Node readNode(folder, 1);

    std::string one;
    readNode.get_value("one", one);

    bool two;
    readNode.get_value("two", two);

    int three;
    readNode.get_value("three", three);

    double four;
    readNode.get_value("four", four);

    printf("One: %s\nTwo: %d\nThree: %d\nFour: %.3f\n", one.c_str(), two, three, four);
}