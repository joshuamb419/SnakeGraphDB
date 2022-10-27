#include <string>
#include <chrono>
#include <stdio.h>
#include "Node.h"
#include "AsciiControlCodes.h"

int main(int argc, char** argv){
    int runs = atoi(argv[1]);
    
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    for(int i = 0; i < runs; i++) {
        std::string folder = "./db_folder/";
        int id = 1;
        Node *node = new Node(folder, id);
        node->set_value("test", std::to_string(i));
        node->dump_data();

        std::string name = node->get_name();
        std::string rep = node->get_value("recipie");
        std::string ingred = node->get_value("ingredients");
        node->dump_data();
        delete node;
    }

    auto end = std::chrono::steady_clock::now();

    std::printf("\nAvg time across %d runs: %.4fms\n\n", runs, double(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / runs);
}