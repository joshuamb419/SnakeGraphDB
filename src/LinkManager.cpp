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

void LinkManager::addLink(int src, Node* dest, std::string linkTitle) {
    Link* link = (Link*) malloc(sizeof(struct Link));
    link->dest = dest;
    link->destId = dest->getId();
    link->title = linkTitle;

    link_map.emplace(src, link);
    dest_set.insert(dest->getId());
}

inline void LinkManager::addLink(Node* src, Node* dest, std::string linkTitle) {
    addLink(src->getId(), dest, linkTitle);
}

void LinkManager::removeLink(int src, Node* dest, std::string linkTitle) {
    auto its = link_map.equal_range(src);

    for(auto linkIt = its.first; linkIt != its.second; linkIt++) {
        if(linkIt->second->dest == dest && linkIt->second == linkTitle) {
            free(linkIt->second);
            link_map.erase(linkIt);
            break;
        }
    }
}

inline void LinkManager::removeLink(Node* src, Node* dest, std::string linkTitle) {
    removeLink(src->getId(), dest, linkTitle);
}

void LinkManager::removeAllLinks(int nodeId) {
    auto its = link_map.equal_range(nodeId);
    for(auto linkIt = its.first; linkIt != its.second; linkIt++) {
        free(linkIt->second);
    }

    link_map.erase(its.first, its.second);
    dest_set.erase(nodeId);
}

inline void LinkManager::removeAllLinks(Node* node) {
    removeAllLinks(node->getId());
}

std::vector<Node*>& LinkManager::getLinks(int src) {
    auto its = link_map.equal_range(src);
    
    std::vector<Node*> links;

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

inline std::vector<Node*>& LinkManager::getLinks(Node* src) {
    return getLinks(src->getId());
}

std::vector<Node*>& LinkManager::getLinks(int src, std::string linkTitle) {
    auto its = link_map.equal_range(src);
    
    std::vector<Node*> links;

    for(auto linkIt = its.first; linkIt != its.second; linkIt++) {
        if(linkIt->second != linkTitle) continue;

        if(dest_set.count(linkIt->second->destId) == 0) {
            free(linkIt->second);
            link_map.erase(linkIt);
            continue;
        }

        links.push_back(linkIt->second->dest);
    }

    return links;
}

inline std::vector<Node*>& LinkManager::getLinks(Node* src, std::string linkTitle) {
    return getLinks(src->getId(), linkTitle);
}

LinkManager::~LinkManager() {
    for(auto it = link_map.begin(); it != link_map.end(); it++) {
        free(it->second);
    }
}