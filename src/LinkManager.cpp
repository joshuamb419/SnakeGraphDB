#include <string>
#include <vector>
#include <map>

#include "LinkManager.h"
#include "Node.h"

using namespace SnakeGraph;

inline bool operator==(const Link* sgl, const std::string& str) {
    return sgl->title == str;
}

inline bool operator!=(const Link* sgl, const std::string& str) {
    return !(sgl == str);
}