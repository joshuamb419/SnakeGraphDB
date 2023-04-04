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

std::string StringLabel::encodeLabel() {
    std::string encodedLabel = "1";
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += getTitle();
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += value;
    return encodedLabel;
}

std::string IntLabel::encodeLabel() {
    std::string encodedLabel = "2";
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += getTitle();
    encodedLabel += char(A_RECORD_SEP);
    encodedLabel += value;
    return encodedLabel;
}

std::string DoubleLabel::encodeLabel() {
  std::string encodedLabel = "3";
  encodedLabel += char(A_RECORD_SEP);
  encodedLabel += getTitle();
  encodedLabel += char(A_RECORD_SEP);
  encodedLabel += value;
  return encodedLabel;
}
