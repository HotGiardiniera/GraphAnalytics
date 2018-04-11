#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include <omp.h>

#define MAX_NODE_SIZE 10000
#define MAX_THREAD_COUNT 64
/*
    Random Matrix Generator

    @author   Chris Moriano
    @version  0.0.0.1 (Super Alpha)

    This program will generate a random matrix to a text file using the
    Erdos-Renyi method

    The nodes are represented as the colums of the matrix and
    the edges are represented by the rows.
    If there is an edge from A to B it is represented by a 1 otherwise a 0

    By default the program generates a DAG, only the lower portion of the
    matrix is populated.

    The output file consists of:
    First line - size N of the NxN matrix
    Remaining lines: the matrix

    options:
    -f <string>: Custom output filename @Shaheer Haroon
    -w <int>: maximum weight of edge, default is 1 (all edge weights are ints) @Shaheer Haroon
    -t <int>: number of threads
    -s <int>: custom size of matrix, default is 10
    -p <int>: probaility that all node A->B has an edge, default 50
    -d <optional 0>: if this option is 0 we generate an undirectional graph with no self edges
    -v: verbose mode
*/



char* file_name = "rand_m_out.txt";
int matrix_size = -1, probability = -1;
int max_weight = 1;
int thread_count = 1;
bool verbose = false, dag = true;

void get_options(int argc, char* argv[]){
    char c;
    int temp_size, temp_prob;
    while((c = getopt(argc, argv, "s:p:f:vd:w:t:")) != -1){
        switch(c){
            case 'f':
                file_name = optarg;
                break;
            case 'w':
                max_weight = atoi(optarg);
                if(max_weight < 1){
                    printf("Matrix size must be at least 1\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case 't':
                thread_count = atoi(optarg);
                if(thread_count < 1 || thread_count > MAX_THREAD_COUNT){
                    printf("Number of threads specified must be greater than 0 and smaller than %d\n", MAX_THREAD_COUNT);
                    exit(EXIT_FAILURE);
                }
                break;
            case 's':
                temp_size = atoi(optarg);
                if(temp_size < 1 || temp_size > MAX_NODE_SIZE){
                    printf("Matrix size must be greater than 0 or smaller than %d\n", MAX_NODE_SIZE);
                    exit(EXIT_FAILURE);
                }
                matrix_size = temp_size;
                break;
            case 'p':
                temp_prob = atoi(optarg);
                if(temp_prob < 1 || temp_prob > 99){
                    printf("Probability must be between 0 and 100. Given: %d \n", temp_prob);
                    exit(EXIT_FAILURE);
                }
                probability = temp_prob;
                break;
            case 'd':
                if(atoi(optarg) == 0){
                    dag = false;
                }
                break;
            case 'v':
                verbose = true;
                break;
        }
    }
}


int main(int argc, char *argv[]){
    int rand_num;

    get_options(argc, argv);

    if(matrix_size == -1){
        matrix_size = 10;
    }
    if(probability == -1){
        probability = 50;
    }

    if(verbose){
        printf("Matrix size %d\n", matrix_size);
        printf("Probability %d\n", probability);
        printf("Max Edge Weight %d\n", max_weight);
    }

    srand(time(NULL));

    int** matrix = (int **) malloc(matrix_size * sizeof(int *));
    int i;
    int j;
    if(matrix_size){
        matrix[0] = calloc(matrix_size * matrix_size, sizeof(int));
        for(i = 1; i < matrix_size; ++i){
            matrix[i] = matrix[0] + i * matrix_size;
        }
    }

    if(dag){
      #pragma omp parallel num_threads(thread_count)
      {
        for(i=0; i<matrix_size; i++){
            #pragma omp for
            for(j=0; j<i; j++){
                rand_num = rand() % 100;
                if(rand_num < probability){
                    matrix[i][j] = (rand() % max_weight) + 1;
                } else {
                    matrix[i][j] = 0;
                }

            }
        }
      }
    } else {
      #pragma omp parallel num_threads(thread_count)
      {
        #pragma omp for
        for(i=0; i<matrix_size; i++){
            for(j=0; j<matrix_size; j++){
                if(i == j){
                    continue;
                } else {
                    rand_num = rand() % 100;
                    if(rand_num < probability){
                        matrix[i][j] = (rand() % max_weight) + 1;
                    } else {
                        matrix[i][j] = 0;
                    }
                }

            }
        }
      }
    }

    // Write to our file
    FILE *fp = fopen(file_name, "w+");

    if(!fp){
        printf("Error opening output file\n");
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%d\n", matrix_size);;

    for(i=0; i<matrix_size; i++){
        for(j=0; j<matrix_size; j++){
            fputc(matrix[i][j] + '0', fp);
            fputc(0x20, fp);  // Space
            if(verbose){
                printf("%d ", matrix[i][j]);
            }
        }
        fputc('\n', fp);
        if(verbose){
            printf("\n");
        }
    }

    return EXIT_SUCCESS;

}
