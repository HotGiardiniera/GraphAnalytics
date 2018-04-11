#include <stdio.h>
#include <cstdint>
#include <map>
#include <vector>
#include <queue>
#include "Vertex.h"
#include <omp.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <set>


int main(int argc, char *argv[]){
    printf("Starting\n");


    if(argc < 3){
        printf("Invalid parameters. Usage: ./bfs inputfile thread_count\n");
        return EXIT_FAILURE;
    }

    // -----  Program args  -----------------
    int file_val, m_size, thread_count;

    thread_count = atoi(argv[argc-1]);

    char *file_name = argv[argc-2];

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
    std::queue<uint32_t> Ready;
    std::vector<Vertex *> dist(m_size);
    std::vector<Vertex *> parent(m_size);


    printf("Reading Matrix\n");
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
    dist[0] = 0;

    struct timeval start, end;
    long mtime, secs, usecs; 


    uint32_t frontier;  // the number in our frontier, we will stop when the frontier is empty
    std::vector<Vertex *> frontier_queue;
    std::vector<Vertex *> new_frontier_queue;
    frontier_queue = adjacency_hash[0]; // first frontier from our start vertex
    G[0].colour = BLACK;
    for(size_t i=0; i<frontier_queue.size(); i++)
        frontier_queue[i]->colour = GREY;

    // Fill our frontier with the 
    frontier = frontier_queue.size();

    printf("Starting BFS\n");
    gettimeofday(&start, NULL);


    // Top down approach
    while(frontier > 0){
        // Clear our frontier count at each loop
        frontier = 0;
        #pragma omp parallel num_threads(thread_count)
        {   
            std::vector<Vertex *> temp;  // Each thread has it's own copy of neighbors
            int num_threads = omp_get_num_threads();
            int thread_id = omp_get_thread_num();
            for(size_t i=thread_id; i<frontier_queue.size(); i+=num_threads){
                if(frontier_queue[i]->colour == GREY){
                    frontier_queue[i]->colour = BLACK;
                    temp = adjacency_hash[frontier_queue[i]->name];
                    for(size_t j=0; j<temp.size(); j++){
                        if(temp[j]->colour == WHITE){
                            // printf("temp name: %d\n", temp[j]->name);
                            temp[j]->colour = GREY;
                            #pragma omp critical
                            {
                                frontier += 1;
                                new_frontier_queue.push_back(temp[j]);
                            }
                        }
                    } 
                }
            }
        }
        frontier_queue.clear();
        frontier_queue.swap(new_frontier_queue);
    }


    gettimeofday(&end, NULL);
    secs  = end.tv_sec  - start.tv_sec;
    usecs = end.tv_usec - start.tv_usec;
    mtime = ((secs) * 1000 + usecs/1000.0) + 0.5;
    printf("Elapsed BFS time: %ld millisecs\n", usecs);

    printf("Done Searching!\n");
    printf("Verifying Graph\n");
    for(int i=0; i<m_size; i++){
        assert(G[i].colour == BLACK);
    }

    return 0;
}