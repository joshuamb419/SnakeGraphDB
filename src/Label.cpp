#include <random>
#include <stdexcept>
#include <string>

#include "AsciiControlCodes.h"
#include "Label.h"

using namespace SnakeGraph;

std::string Label::getTitle() {
    return title;
}

std::string Label::encodeLabel() {
    std::string encodedLabel = "0";
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += getTitle();
    return encodedLabel;
}

StringLabel::StringLabel(std::string& encodedString) {
    int sepIndex = encodedString.find(A_RECORD_SEP);
    title = encodedString.substr(sepIndex).c_str();
    encodedString.erase(0, sepIndex + 1);

    value = encodedString;
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

std::string StringLabel::encodeLabel() {
    std::string encodedLabel = "1";
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += getTitle();
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += value;
    return encodedLabel;
}

IntLabel::IntLabel(std::string& encodedString) {
    int sepIndex = encodedString.find(A_RECORD_SEP);
    title = encodedString.substr(sepIndex).c_str();
    encodedString.erase(0, sepIndex + 1);

    value = std::atoi(encodedString.c_str());
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

std::string IntLabel::encodeLabel() {
    std::string encodedLabel = "2";
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += getTitle();
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += value;
    return encodedLabel;
}

DoubleLabel::DoubleLabel(std::string& encodedString) {
    int sepIndex = encodedString.find(A_RECORD_SEP);
    title = encodedString.substr(sepIndex).c_str();
    encodedString.erase(0, sepIndex + 1);

    value = std::atof((encodedString.c_str()));
}

DoubleLabel::DoubleLabel(std::string title, double value) : Label(title) {
    this->value = value;
}

std::string DoubleLabel::encodeLabel() {
    std::string encodedLabel = "3";
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += getTitle();
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += value;
    return encodedLabel;
}
