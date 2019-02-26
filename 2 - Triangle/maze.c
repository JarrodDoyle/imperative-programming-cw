#include <stdio.h>
#include <stdlib.h>

// Returns a list of node ids after combining two regions
int join_regions(int joined[], int lr, int r[], int ls, int s[]) {
    for (int i=0; i<(lr+ls); i++) {
        if (i < lr) {
            joined[i] = r[i];
        }
        else {
            joined[i] = s[i % lr];
        }
    }
}

// Generate a list of unique ID's based on the size of the maze
int * gen_nodes(int width, int height) {
    int num_nodes = ((width + 1) / 2) * ((height + 1) / 2);
    int node_ids[num_nodes];
    for (int i=0; i<num_nodes; i++) {
        node_ids[i] = i;
    }
    return node_ids;
}

// Generate an adjacency matrix using random edge weights
int * gen_adjacency_matrix(int width, int height) {
    width = (width + 1) / 2;
    height = (height + 1) / 2;
    int num_nodes = width * height;
    int adj_matrix[num_nodes][num_nodes];
    int weight;
    for (int i=0; i<num_nodes; i++) {
        // If node isn't in right column create a connection to right
        if (i % width != width - 1) {
            weight = rand();
            adj_matrix[i][i+1] = weight;
            adj_matrix[i+1][i] = weight;
        }
        // If node isn't in last row create connection to below node
        if (i < num_nodes - width) {
            weight = rand();
            adj_matrix[i][i+width] = weight;
            adj_matrix[i+width][i] = weight;
        }
    }
    return adj_matrix;
}

// Convert adjacency matrix into a character rpresentation
char * convert(int graph[]){
    ;
}

int * calc_edges(int w, int h, int graph[h][w], int num_nodes) {
    int edges[num_nodes][3];
    int num_edges = 0;
    for (int i=0; i<num_nodes; i++) {
        for (int j=i+1; j<num_nodes; j++) {
            if (graph[i][j] > 0) {
                edges[num_edges][0] = i;
                edges[num_edges][1] = j;
                edges[num_edges][2] = graph[i][j];
            }
        }
    }
    return edges;
}

int sort_edges(int edges[], int low, int high) {
    if (low < high) {
        ;
    }
}

// Kruskals function
int * mst(int w, int h, int graph[h][w], int num_nodes) {
    // Initialise regions (used to ensure no cycles)
    int regions[num_nodes];
    int num_regions = num_nodes;
    for (int i=0; i<num_nodes; i++) {
        regions[i] = ;
    }
    int *edges = calc_edges(w, h, graph, num_nodes);
    while (num_regions > 1) {
        ;
    }
}

int main() {
    ;
}
