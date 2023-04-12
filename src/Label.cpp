#include <algorithm>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "AsciiControlCodes.h"
#include "Label.h"

using namespace SnakeGraph;

std::string Label::getTitle() {
    return title;
}

std::vector<unsigned char> Label::encodeLabel() {
    std::vector<unsigned char> encodedLabel;
    encodedLabel.push_back(char(48));
    encodedLabel.insert(encodedLabel.end(), title.begin(), title.end());
    return encodedLabel;
}

StringLabel::StringLabel(std::vector<unsigned char>& encodedString) {
    auto it = std::find(encodedString.begin(), encodedString.end(), char(A_RECORD_SEP));
    title = std::string(encodedString.begin(), it);
    it++;
    encodedString.erase(encodedString.begin(), it);

    value = std::string(encodedString.begin(), encodedString.end());
}

StringLabel::StringLabel(std::string title, std::string value) : Label(title) {
    this->value = value;
}

std::string& StringLabel::getValue() {
    return value;
}

void StringLabel::setValue(std::string value) {
    this->value = value;
}

std::vector<unsigned char> StringLabel::encodeLabel() {
    std::vector<unsigned char> encodedLabel;
    encodedLabel.push_back(char(49));
    encodedLabel.insert(encodedLabel.end(), title.begin(), title.end());
    encodedLabel.push_back(char(A_RECORD_SEP));
    encodedLabel.insert(encodedLabel.end(), value.begin(), value.end());
    return encodedLabel;
}

IntLabel::IntLabel(std::vector<unsigned char>& encodedString) {
    auto it = std::find(encodedString.begin(), encodedString.end(), char(A_RECORD_SEP));
    title = std::string(encodedString.begin(), it);
    it++;
    encodedString.erase(encodedString.begin(), it);

    value = *((int32_t*) encodedString.data());
}

IntLabel::IntLabel(std::string title, int32_t value) : Label(title) {
    this->value = value;
}

int32_t& IntLabel::getValue() {
    return value;
}

void IntLabel::setValue(int32_t value) {
    this->value = value;
}

std::vector<unsigned char> IntLabel::encodeLabel() {
    std::vector<unsigned char> encodedLabel;
    encodedLabel.push_back(char(50));
    encodedLabel.insert(encodedLabel.end(), title.begin(), title.end());
    encodedLabel.push_back(char(A_RECORD_SEP));

    unsigned char* valuePointer = reinterpret_cast<unsigned char*>(&value);
    encodedLabel.insert(encodedLabel.end(), valuePointer, valuePointer + sizeof(value));
    return encodedLabel;
}

DoubleLabel::DoubleLabel(std::vector<unsigned char>& encodedString) {
    auto it = std::find(encodedString.begin(), encodedString.end(), char(A_RECORD_SEP));
    title = std::string(encodedString.begin(), it);
    it++;
    encodedString.erase(encodedString.begin(), it);

    value = *((double*) encodedString.data());
}

DoubleLabel::DoubleLabel(std::string title, double value) : Label(title) {
    this->value = value;
}

std::vector<unsigned char> DoubleLabel::encodeLabel() {
    std::vector<unsigned char> encodedLabel;
    encodedLabel.push_back(char(51));
    encodedLabel.insert(encodedLabel.end(), title.begin(), title.end());
    encodedLabel.push_back(char(A_RECORD_SEP));

    unsigned char* valuePointer = reinterpret_cast<unsigned char*>(&value);
    encodedLabel.insert(encodedLabel.end(), valuePointer, valuePointer + sizeof(value));
    return encodedLabel;
}
