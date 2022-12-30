#include "SGLinkManager.h"
#include "SGNode.h"

struct SGLink {
    SGNode* dest;
    std::string title;
};

void SGLinkManager::addLink(int src, SGNode* dest, std::string title) {
    SGLink* link = (SGLink*) malloc(sizeof(struct SGLink));
    link->dest = dest;
    link->title = title;

    link_map.emplace(src, link);
}

inline void SGLinkManager::addLink(SGNode* src, SGNode* dest, std::string title) {
    addLink(src->getId(), dest, title);
}