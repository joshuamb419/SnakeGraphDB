#ifndef SG_LINK_MANAGER
#define SG_LINK_MANAGER

#include <set>
#include <vector>

#include "Label.h"
#include "Node.h"

namespace SnakeGraph {
    struct Link {
        int id;
        int sourceId;
        int destId;
        std::string title;
        std::vector<Label*> labels;
    };

    class LinkManager {
        private:
            std::set<Link*> links;
        
        protected:
            LinkManager();
          
            // Link from source to dest is created with title linkTitle
            void addLink(int src, Node* dest, std::string linkTitle);
            inline void addLink(Node* src, Node* dest, std::string linkTitle);
          
            // Link from source to dest with matching link title is removed
            void removeLink(int src, Node* dest, std::string linkTitle);
            inline void removeLink(Node* src, Node* dest, std::string linkTitle);
          
            void removeAllLinks(int nodeId);
            inline void removeAllLinks(Node* node);
          
            std::vector<Node*>& getLinks(int src);
            inline std::vector<Node*>& getLinks(Node* src);
            std::vector<Node*>& getLinks(int src, std::string linkTitle);
            inline std::vector<Node*>& getLinks(Node* src, std::string linkTitle);
          
        public:
            ~LinkManager();
        
            friend class SnakeGraph;
    };
}

#endif