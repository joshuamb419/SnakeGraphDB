#include <iostream>
#include "Node.h"


int main(int argc, char** argv){
    Node node("Hello World");
    std::cout << node.get_id() << "\n";
}