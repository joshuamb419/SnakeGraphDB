#ifndef SG_LABEL_BUILDER
#define SG_LABEL_BUILDER

#include <vector>

#include "Label.h"

namespace SnakeGraph {
    class LabelDecoder {
        public:
            static Label* decodeLabel(std::vector<unsigned char> encodedString);
    };
}

#endif