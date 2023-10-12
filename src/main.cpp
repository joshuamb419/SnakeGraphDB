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
    Label* intLabel = new IntLabel("Label 1", 15);
    Label* strLabel = new StringLabel("Label 2", "15");

    printf("int equals string: %d\n", intLabel->equals(strLabel));

    auto intStr = intLabel->encodeLabel();
    auto strStr = strLabel->encodeLabel();

    Label* newInt = LabelDecoder::decodeLabel(intStr);
    Label* newStr = LabelDecoder::decodeLabel(strStr);
    
    printf("new int = old int: %d\n", intLabel->equals(newInt));
    printf("new str = old str: %d\n", strLabel->equals(newStr));
    printf("new int = new str: %d\n", newInt->equals(newStr));

    delete intLabel;
    delete strLabel;
    delete newInt;
    delete newStr;
}
