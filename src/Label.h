#ifndef SG_LABEL
#define SG_LABEL

#include <string>

namespace SnakeGraph {

/*
 * Notes on the intended usages of Label:
 * - Labels are supposed to be fast access data that stays in memory for sorting between nodes
 * - Labels are not meant to be directly created or manipulated by users, this datatype should stay within the SnakeGraph library
 * - Labels can be compared to eachother, follows normal C++ conventions for comparrisons
 *
 * Label Types:
 * 0: No data
 * 1: String data
 * 2: 32bit Int
 * 3: Double
 */

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