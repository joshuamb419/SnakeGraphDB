#include <cstdio>
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>

#include "LabelDecoder.h"
#include "Label.h"

using namespace SnakeGraph;

int main(int argc, char** argv){
    for(int i = -10; i < 10; i++){
        if(i) {
          std::printf("true\n");
        } else {
          std:: printf("false\n");
        }
    }
}
