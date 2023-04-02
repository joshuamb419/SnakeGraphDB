#ifndef SG_LINK_MANAGER
#define SG_LINK_MANAGER

#include <map>
#include <vector>
#include <set>

#include "Node.h"

namespace SnakeGraph {
    class LinkManager {
    private:
      // Provides all links indexed by source node
      std::multimap<int, struct Link*> link_map;
    
      // Stores all destination nodes, prevents links from being valid if the
      // destination has been removed from the manager
      std::set<int> dest_set;
    
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