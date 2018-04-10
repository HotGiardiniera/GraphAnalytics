#ifndef VERTEX_H
#define VERTEX_H

#include <stdint.h>
enum COLOR { WHITE, GREY, BLACK };

class Vertex
{
public:
    uint32_t indegree, name;
    enum COLOR colour;  //British and American English compatibility
    Vertex(uint32_t);
    Vertex();
    ~Vertex(){};
};

#endif