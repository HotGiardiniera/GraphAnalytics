#include "Vertex.h"

Vertex::Vertex(uint32_t _name){
    indegree = 0;
    name = _name;
}

Vertex::Vertex(){
    indegree = 0;
    name = 0;
}