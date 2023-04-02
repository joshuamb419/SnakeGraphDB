#ifndef SG_LABEL
#define SG_LABEL

#include <string>

namespace SnakeGraph {
    class Label {
        private:
            int labelType = 0;
            std::string title;
    };
    
    class StringLabel : Label {
        private:
            std::string value;
    };
    
    class IntLabel : Label {
        private:
            int32_t value;
    };
    
    class DoubleLabel : Label {
        private:
            double value;
    };
}

#endif