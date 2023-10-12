#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include "LabelDecoder.h"
#include "Label.h"
#include "AsciiControlCodes.h"

using namespace SnakeGraph;

Label* LabelDecoder::decodeLabel(std::vector<unsigned char> encodedString) {
    int labelType = encodedString.at(0) - 48;
    encodedString.erase(encodedString.begin());

    switch(labelType) {
        case 0:
            return new Label(std::string(encodedString.begin(), encodedString.end()));
            break;
        case 1:
            return new StringLabel(encodedString);
            break;
        case 2:
            return new IntLabel(encodedString);
            break;
        case 3:
            return new DoubleLabel(encodedString);
            break;
        case 4:
            return new BoolLabel(encodedString);
            break;
        default:
            throw std::invalid_argument("Encoded Label type invalid");
    }
}