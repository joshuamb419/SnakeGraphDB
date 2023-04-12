#include <cstdio>
#include <string>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include "Label.h"
#include "LabelDecoder.h"

using namespace SnakeGraph;

int main(int argc, char** argv){
    Label* testLabel = new IntLabel("FavQuote", 1);
    std::vector<unsigned char> encodedLabel = testLabel->encodeLabel();

    Label* decodedLabel = LabelDecoder::decodeLabel(encodedLabel);
    std::printf("Original: ");
    for(unsigned char c : encodedLabel){
        std::printf("%c", c);
    }

    std::printf("\nRecoded: ");
    for(unsigned char c : decodedLabel->encodeLabel()) {
        std::printf("%c", c);
    }
    std::printf("\n");
    
}
