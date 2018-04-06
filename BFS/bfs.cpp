#include <stdio.h>
#include <cstdint>
#include <map>
#include <vector>
#include <queue>
#include "Vertex.h"


int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Invalid parameters. Usage: ./bfs inputfile\n");
        return EXIT_FAILURE;
    }

    // -----  Program args  -----------------
    int file_val, m_size;
    char *file_name = argv[argc-1];

    FILE * file_pointer = NULL;
    file_pointer = fopen(file_name, "r");
    if(file_pointer == NULL){
        printf("Error opening file %s\n", file_name);
        return EXIT_FAILURE;
    }
    fscanf(file_pointer, "%d", &m_size);
    int** matrix = new int*[m_size];
    if (m_size){
        matrix[0] = new int[m_size * m_size];
        for (int i = 1; i < m_size; ++i){
            // hash[i] = 0;
            matrix[i] = matrix[0] + i * m_size;
        }
    }

    // Data structures for our top sort
    std::vector<Vertex> G(m_size);
    std::map<uint32_t, std::vector<Vertex *> > adjacency_hash;
    std::queue<int> Ready;
    std::vector<Vertex *> temp;


    // Generic Matrix reader (inneficient for our DAG so we will ignore this in timings) 
    for(int i=0; i<m_size; i++){
        G[i].name = i;
        for(int j=0; j<m_size; j++){
            fscanf(file_pointer, "%d", &file_val);
            matrix[i][j] = file_val;
            if(file_val == 1){
                G[i].indegree += 1;
                adjacency_hash[j].push_back(&G[i]);
            }
        }
    }

    for(int i=0; i<m_size; i++){
        if(G[i].indegree == 0){
            Ready.push(i);
        }
    }

    uint32_t vertex_location;

    while(!Ready.empty()){
        vertex_location = Ready.front();
        Ready.pop();
        printf("Vertex: %d\n", G[vertex_location].name);

        temp = adjacency_hash[vertex_location];

        for(size_t i=0; i<temp.size(); i++){
            temp[i]->indegree -= 1;
            if(temp[i]->indegree == 0){
                Ready.push(temp[i]->name);
            }
        }
    }

    return 0;
}