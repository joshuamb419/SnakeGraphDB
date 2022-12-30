#ifndef SG_LINK_MANAGER
#define SG_LINK_MANAGER

#include <unordered_map>
#include <vector>

#include "SGNode.h"

class SGLinkManager {
    private:
        std::unordered_multimap<int, struct SGLink*> link_map;
    
    protected:
        SGLinkManager();

        void addLink(int src, SGNode* dest, std::string title);
        inline void addLink(SGNode* src, SGNode* dest, std::string title);
        
        void removeLink(int src, SGNode* dest, std::string title);
        inline void removeLink(SGNode* src, SGNode* dest, std::string title);

        void removeAllLinks(int nodeId);
        inline void removeAllLinks(SGNode* node);

        std::vector<SGNode*> getLinks(int src);
        inline std::vector<SGNode*> getLinks(SGNode* src);
        std::vector<SGNode*> getLinks(int src, std::string title);
        inline std::vector<SGNode*> getLinks(SGNode* src, std::string title);
    
    friend class SnakeGraph;
};

#endif