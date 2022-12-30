#ifndef SG_LINK_MANAGER
#define SG_LINK_MANAGER

#include <unordered_map>
#include <vector>
#include <set>

#include "SGNode.h"

class SGLinkManager {
    private:
        std::unordered_multimap<int, struct SGLink*> link_map;
        std::set<int> dest_set;
    
    protected:
        SGLinkManager();

        void addLink(int src, SGNode* dest, std::string linkTitle);
        inline void addLink(SGNode* src, SGNode* dest, std::string linkTitle);
        
        void removeLink(int src, SGNode* dest, std::string linkTitle);
        inline void removeLink(SGNode* src, SGNode* dest, std::string linkTitle);

        void removeAllLinks(int nodeId);
        inline void removeAllLinks(SGNode* node);

        std::vector<SGNode*>& getLinks(int src);
        inline std::vector<SGNode*>& getLinks(SGNode* src);
        std::vector<SGNode*>& getLinks(int src, std::string linkTitle);
        inline std::vector<SGNode*>& getLinks(SGNode* src, std::string linkTitle);

    public:
        ~SGLinkManager();
    
    friend class SnakeGraph;
};

#endif