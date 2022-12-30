#include <string>
#include <vector>

#include "SGLinkManager.h"
#include "SGNode.h"

struct SGLink {
    SGNode* dest;
    int destId;
    std::string title;
};

void SGLinkManager::addLink(int src, SGNode* dest, std::string linkTitle) {
    SGLink* link = (SGLink*) malloc(sizeof(struct SGLink));
    link->dest = dest;
    link->destId = dest->getId();
    link->title = linkTitle;

    link_map.emplace(src, link);
    dest_set.insert(dest->getId());
}

inline void SGLinkManager::addLink(SGNode* src, SGNode* dest, std::string linkTitle) {
    addLink(src->getId(), dest, linkTitle);
}

void SGLinkManager::removeLink(int src, SGNode* dest, std::string linkTitle) {
    auto its = link_map.equal_range(src);

    for(auto linkIt = its.first; linkIt != its.second; linkIt++) {
        if(linkIt->second->dest == dest && linkIt->second->title == linkTitle) {
            free(linkIt->second);
            link_map.erase(linkIt);
            break;
        }
    }
}

inline void SGLinkManager::removeLink(SGNode* src, SGNode* dest, std::string linkTitle) {
    removeLink(src->getId(), dest, linkTitle);
}

void SGLinkManager::removeAllLinks(int nodeId) {
    auto its = link_map.equal_range(nodeId);
    for(auto linkIt = its.first; linkIt != its.second; linkIt++) {
        free(linkIt->second);
    }

    link_map.erase(its.first, its.second);
    dest_set.erase(nodeId);
}

inline void SGLinkManager::removeAllLinks(SGNode* node) {
    removeAllLinks(node->getId());
}

std::vector<SGNode*>& SGLinkManager::getLinks(int src) {
    auto its = link_map.equal_range(src);
    
    std::vector<SGNode*> links;

    for(auto linkIt = its.first; linkIt != its.second; linkIt++) {
        if(dest_set.count(linkIt->second->destId) == 0) {
            free(linkIt->second);
            link_map.erase(linkIt);
            continue;
        }

        links.push_back(linkIt->second->dest);
    }

    return links;
}

inline std::vector<SGNode*>& SGLinkManager::getLinks(SGNode* src) {
    return getLinks(src->getId());
}

std::vector<SGNode*>& SGLinkManager::getLinks(int src, std::string linkTitle) {
    auto its = link_map.equal_range(src);
    
    std::vector<SGNode*> links;

    for(auto linkIt = its.first; linkIt != its.second; linkIt++) {
        if(linkIt->second->title != linkTitle) continue;

        if(dest_set.count(linkIt->second->destId) == 0) {
            free(linkIt->second);
            link_map.erase(linkIt);
            continue;
        }

        links.push_back(linkIt->second->dest);
    }

    return links;
}

inline std::vector<SGNode*>& SGLinkManager::getLinks(SGNode* src, std::string linkTitle) {
    return getLinks(src->getId(), linkTitle);
}

SGLinkManager::~SGLinkManager() {
    for(auto it = link_map.begin(); it != link_map.end(); it++) {
        free(it->second);
    }
}