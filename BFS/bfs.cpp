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

    // ------------- TODO ---------------
    // Use the CLRS BFS with 0 as our starting matrix NOTE: our ltgen will need an option to make the first node our source 

    // Data structures for our top sort
    std::vector<Vertex> G(m_size);
    std::map<uint32_t, std::vector<Vertex *> > adjacency_hash;
    std::queue<uint32_t> Ready;
    std::vector<Vertex *> temp;
    std::vector<Vertex *> dist(m_size);
    std::vector<Vertex *> parent(m_size);


    // Generic Matrix reader
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

    fclose(file_pointer);
    delete [] matrix[0];
    delete [] matrix;

    Vertex *src = &G[0];
    src->colour = GREY;
    Ready.push(0);  // This is our source
    dist[0] = 0;



    uint32_t vertex_location;

    while(!Ready.empty()){
        vertex_location = Ready.front();
        Ready.pop();

        temp = adjacency_hash[vertex_location];

        for(size_t i=0; i<temp.size(); i++){
            printf(".");
            if(temp[i]->colour == WHITE){
                dist[temp[i]->name] = dist[vertex_location] + 1;
                parent[temp[i]->name] = &G[vertex_location];
                temp[i]->colour = GREY;
                Ready.push(temp[i]->name);
                printf("\b");
            }
        }
        G[vertex_location].colour = BLACK;
    }
    printf("Done Searching!\n");

    return 0;
}