#ifndef SG_LABEL_BUILDER
#define SG_LABEL_BUILDER

#include "Label.h"

namespace SnakeGraph {
    class LabelBuilder {
        public:
            static Label loadLabel(std::string encodedString);
    };
}

#endif