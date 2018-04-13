#include <stdio.h>
#include <cstdint>
#include <map>
#include <vector>
#include <queue>
#include "PageNode.h"
#include <omp.h>


int main(int argc, char *argv[]){

    if(argc < 3){
        printf("Invalid parameters. Usage: ./pagerank inputfile thread_count\n");
        return EXIT_FAILURE;
    }

    float dapening_f = 0.85; 

    // -----  Program args  -----------------
    int file_val, m_size, thread_count = atoi(argv[argc-1]);
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
            matrix[i] = matrix[0] + i * m_size;
        }
    }

    std::vector<PageNode> G(m_size);  // Page graph
    std::map<uint32_t, std::vector<PageNode *> > page_ref_hash;  // Adjacency matrix for Pages that refernce a certain page


    // Read our matrix and set up our initial ranks
    for(int i=0; i<m_size; i++){
        G[i].name = i;
        for(int j=0; j<m_size; j++){
            fscanf(file_pointer, "%d", &file_val);
            matrix[i][j] = file_val;
            if(file_val == 1){
                G[i].indegree += 1;
                G[j].outdegree += 1;
                page_ref_hash[i].push_back(&G[j]);
            }
        }
    }

    // Set our initial ranks
    PageNode *temp, *temp_ref;

    for(int i=0; i<m_size; i++){
        temp = &G[i];
        for(size_t j=0; j<page_ref_hash[i].size(); j++){ // Could use iterators but I don't want the overhead
            temp_ref = page_ref_hash[i][j];
            temp->rank += 1.0/temp_ref->outdegree;
        }
    }

   // iterate calculate our ranks
    int iteration_count = 45; 
    float running_rank_sum = 0.0;
    float *interation_vector = (float*)calloc(m_size, sizeof(float));

    // BELOW CODE IS WORKING AND MAY WORK FASTER; WORSE COME TO WORSE WE HAVE A MULTITHREADED VERSION

    while(iteration_count > 0){

        for(int i=0; i<m_size; i++){
            running_rank_sum = 0.0;
            #pragma omp parallel num_threads(thread_count) reduction(+: running_rank_sum)
            {   
                PageNode *local_ref;
                int num_threads = omp_get_num_threads();
                int thread_id = omp_get_thread_num();
                for(size_t j=thread_id; j<page_ref_hash[i].size(); j+=num_threads){  // find things that reference me
                    local_ref = page_ref_hash[i][j];
                    running_rank_sum += (local_ref->rank)/local_ref->outdegree;
                }
            } 
            interation_vector[i] = (1 - dapening_f) + dapening_f * running_rank_sum;
        }

        #pragma omp parallel for num_threads(thread_count)
        for(int i=0; i<m_size; i++){
            G[i].rank = interation_vector[i];
        }

        iteration_count--;
    }

    //Debug print our pages
    for(int i=0; i<m_size; i++){
        printf("Page: %d rank: %f\n", G[i].name, G[i].rank);
    }

    return EXIT_SUCCESS;
}