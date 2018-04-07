#include "PageNode.h"

PageNode::PageNode(uint32_t _name){
    indegree = 0;
    name = _name;
    rank = 0.0;
}

PageNode::PageNode(){
    indegree = 0;
    name = 0;
    rank = 0.0;
}