#include <stdexcept>
#include <string>

#include "LabelBuilder.h"
#include "Label.h"
#include "AsciiControlCodes.h"

using namespace SnakeGraph;

Label LabelBuilder::loadLabel(std::string encodedString) {
    int sepIndex = encodedString.find(A_RECORD_SEP);
    int labelType = std::atoi(encodedString.substr(sepIndex).c_str());
    encodedString.erase(0, sepIndex + 1);

    switch(labelType) {
        case 0:
            return Label(encodedString);
            break;
        case 1:
            return StringLabel(encodedString);
            break;
        case 2:
            return IntLabel(encodedString);
            break;
        case 3:
            return DoubleLabel(encodedString);
            break;
        default:
            throw std::invalid_argument("Encoded Label String type invalid");
    }
}