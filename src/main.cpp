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
    Label* testLabel = new StringLabel("FavQuote", "Hello There");
    std::string encodedLabel = testLabel->encodeLabel();

    Label* decodedLabel = LabelDecoder::decodeLabel(testLabel->encodeLabel());
    std::printf("Original: %s\nRemade: %s",
                encodedLabel.c_str(),
                decodedLabel->encodeLabel().c_str());
    
}
