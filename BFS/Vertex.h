#ifndef VERTEX_H
#define VERTEX_H

#include <stdint.h>

class Vertex
{
public:
    uint32_t indegree, name;
    Vertex(uint32_t);
    Vertex();
    ~Vertex(){};
};

#endif