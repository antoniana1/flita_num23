#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

bool* visited;

typedef struct {
    int64_t node;
    int64_t** matrix;
} Graph;

int64_t countNodes(FILE *file){
    int64_t numOfNodes = 0;
    int64_t first, second;
    while (fscanf(file, "%lld %lld", &first, &second) != EOF) {
        if (first > numOfNodes)
            numOfNodes = first;
        if (second > numOfNodes)
            numOfNodes = second;
    }
    return numOfNodes;
}

Graph* initializeGraph(int64_t nodes) {
    Graph *n_g = malloc(nodes * sizeof(Graph));
    n_g -> matrix = (int64_t**)malloc( nodes * sizeof(int64_t*));
    n_g -> node = nodes;
    for(int64_t i = 0; i < nodes; i++) {
        n_g -> matrix[i] = (int64_t*)malloc( nodes * sizeof(int64_t ));
        for(int64_t j = 0; j < nodes; j++) {
            n_g->matrix[i][j] = 0;
        }
    }
    return n_g;
}

void createGraph(Graph* graph, FILE* file1) {
    int64_t first, second;
    int64_t startNode = 0;
    while (fscanf(file1, "%lld %lld", &first, &second) != EOF) {
        if (first > startNode)
            startNode = first;
        if (second > startNode)
            startNode = second;
        graph -> matrix[first - 1][second - 1] = 1;
        graph -> matrix[second - 1][first - 1] = 1;
    }
}

void printGraph(Graph* graph) {
    for(int64_t i = 0; i < graph -> node; i++) {
        for(int64_t j = 0; j < graph -> node; j++) {
            printf("%lld ", graph -> matrix[i][j]);
        }
        printf("\n");
    }
}

void saveAsDot(Graph* graph) {
    FILE* file2 = fopen("forgraph.dot", "w");
    fprintf(file2, "graph{\n");
    for(int64_t i = 0; i < graph -> node; i++) {
        for(int64_t j = i; j < graph -> node; j++) {
            if(graph -> matrix[i][j] == 1) {
                fprintf(file2, "%lld -- %lld;\n", i + 1, j + 1);
            }
        }
    }
    fprintf(file2, "}\n");
    fclose(file2);
    system("dot -Tpng -O forgraph.dot");
}

void DFS(int64_t i, Graph* graph) {
    visited[i] = true;
    for (int64_t j = 0; j < graph -> node; j++) {
        if (graph -> matrix[i][j] != 0 && !visited[j]) {
            DFS(j, graph);
        }
    }
}

bool isConnected(Graph *graph, int64_t node) {
    visited = malloc(node * sizeof(bool));
    for (int64_t i = 0; i < graph -> node; i++) {
        visited[i] = false;
    }
    DFS(0, graph);
    for (int64_t i = 0; i < graph -> node; i++) {
        if (!visited[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    FILE *file1 = fopen("forgraph.txt", "r");
    int64_t nodes = countNodes(file1);
    Graph *graph = initializeGraph(nodes);
    fclose(file1);
    FILE *file2 = fopen("forgraph.txt", "r");
    createGraph(graph, file2);
    printf("Node count: %lld\n", nodes);
    if (isConnected(graph, nodes)) {
        printf("The graph is connected\n");
    } else {
        printf("The graph is not connected\n");
    }
    printGraph(graph);
    fclose(file2);
    saveAsDot(graph);
    return 0;
}
