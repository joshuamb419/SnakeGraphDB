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

const bool Label::equals(const Label* label) {
    if(this->type != label->type) return false;

    switch(this->type) {
        case 0:
            return this->title == label->title;
        case 1:
            return ((StringLabel*) this)->getValue() == ((StringLabel*) label)->getValue();
        case 2:
            return ((IntLabel*) this)->getValue() == ((IntLabel*) label)->getValue();
        case 3:
            return ((DoubleLabel*) this)->getValue() == ((DoubleLabel*) label)->getValue();
        case 4:
            return ((BoolLabel*) this)->getValue() == ((BoolLabel*) label)->getValue();
        default:
            throw std::invalid_argument("Label type invalid");
    }
}

const bool Label::equals(const SnakeGraph::Label &label) {
    return this->equals(&label);
}

StringLabel::StringLabel(std::vector<unsigned char>& encodedString) {
    auto it = std::find(encodedString.begin(), encodedString.end(), char(A_RECORD_SEP));
    title = std::string(encodedString.begin(), it);
    it++;
    encodedString.erase(encodedString.begin(), it);

    value = std::string(encodedString.begin(), encodedString.end());
    type = 1;
}

StringLabel::StringLabel(std::string title, std::string value) : Label(title, 1) {
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
    type = 2;
}

IntLabel::IntLabel(std::string title, int32_t value) : Label(title, 2) {
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
    type = 3;
}

DoubleLabel::DoubleLabel(std::string title, double value) : Label(title, 3) {
    this->value = value;
}

double& DoubleLabel::getValue() {
    return this->value;
}

void DoubleLabel::setValue(double value) {
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

BoolLabel::BoolLabel(std::vector<unsigned char> &encodedString) {
    auto it = std::find(encodedString.begin(), encodedString.end(), char(A_RECORD_SEP));
    title = std::string(encodedString.begin(), it);
    it++;
    encodedString.erase(encodedString.begin(), it);

    value = *((bool*) encodedString.data());
    type = 4;
}

BoolLabel::BoolLabel(std::string title, bool value) : Label(title, 4) {
    this->value = value;
}

bool& BoolLabel::getValue() {
    return this->value;
}

void BoolLabel::setValue(bool value) {
    this->value = value;
}

std::vector<unsigned char> BoolLabel::encodeLabel() {
    std::vector<unsigned char> encodedLabel;
    encodedLabel.push_back(char(51));
    encodedLabel.insert(encodedLabel.end(), title.begin(), title.end());
    encodedLabel.push_back(char(A_RECORD_SEP));

    unsigned char* valuePointer = reinterpret_cast<unsigned char*>(&value);
    encodedLabel.insert(encodedLabel.end(), valuePointer, valuePointer + sizeof(value));
    return encodedLabel;
}