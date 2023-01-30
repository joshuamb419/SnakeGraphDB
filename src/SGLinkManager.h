#ifndef SG_LINK_MANAGER
#define SG_LINK_MANAGER

#include <map>
#include <vector>
#include <set>

#include "SGNode.h"

class SGLinkManager {
    private:
        // Provides all links indexed by source node
        std::multimap<int, struct SGLink*> link_map;

        // Stores all destination nodes, prevents links from being valid if the
        // destination has been removed from the manager
        std::set<int> dest_set;
    
    protected:
        SGLinkManager();

        // Link from source to dest is created with title linkTitle
        void addLink(int src, SGNode* dest, std::string linkTitle);
        inline void addLink(SGNode* src, SGNode* dest, std::string linkTitle);
        
        // Link from source to dest with matching link title is removed
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