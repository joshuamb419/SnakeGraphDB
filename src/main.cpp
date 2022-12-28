#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "Node.h"

int main(int argc, char** argv){
    // int runs = atoi(argv[1]);

    std::string folder = "./db_folder/";
    std::string name = "test";
    std::string key  = "test";
    std::string value = "thisisavalue";
    int iderz = 1;
    
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

    for(int i = 0; i < 1; i++) {
        Node *node = new Node(folder, 1, name);

        std::vector<unsigned char> data(value.begin(), value.end());
        node->set_value(key, data);

        node->write_data();
    }

    auto end = std::chrono::steady_clock::now();

    std::printf("\nAvg time across %d runs: %.4fms\n\n", 1, double(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) / 1);
}