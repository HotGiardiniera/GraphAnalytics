#ifndef PAGENODE_H
#define PAGENODE_H

#include <stdint.h>

class PageNode
{
public:
    uint32_t indegree, name, outdegree;
    float rank;
    PageNode(uint32_t);
    PageNode();
    ~PageNode(){};
};

#endif