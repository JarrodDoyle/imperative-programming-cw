#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

struct edge {
    int n1;
    int n2;
    int weight;
};
typedef struct edge edge;

// Generate list of potential edges for maze
void gen_edge_list(int w, int h, edge edges[2*w*h-(w+h)]) {
    int num_nodes = w * h;
    int num_edges = 0;
    for (int i=0; i<num_nodes; i++) {
        // Make connection to node to right, unless in last column
        if (i % w != w - 1) {
            edge e = {i, i+1, rand()};
            edges[num_edges] = e;
            num_edges++;
        }
        // Make connection to node below, unless in last row
        if (i < num_nodes - w) {
            edge e = {i, i+w, rand()};
            edges[num_edges] = e;
            num_edges++;
        }
    }
}

// Compare edge structs by weight
int compare_edges(const void *s1, const void *s2) {
    struct edge *e1 = (struct edge *)s1;
    struct edge *e2 = (struct edge *)s2;
    return e1->weight - e2->weight;
}

// Join region with ID2 to region with ID1
void join_regions(int num_nodes, int node_regions[num_nodes], int id1, int id2) {
    // Loop through nodes and change ones with region ID1 to region ID2
    for (int i=0; i<num_nodes; i++) {
        if (node_regions[i] == id2) node_regions[i] = id1;
    }
}

// Generate a minimum spanning tree using Kruskal's
void mst(int w, int h, edge edges[2*w*h-(w+h)], edge new_edges[(w*h)-1]) {
    // Initialize regions to be used for cycle checking
    // node_regions indicates the region node with id == index belongs to
    int node_regions[w * h];
    int num_regions = w * h;
    for (int i=0; i<num_regions; i++) {
        node_regions[i] = i;
    }
    // Sort edges by weight.
    qsort(edges, 2*w*h-(w+h), sizeof(struct edge), compare_edges);
    // Join regions using Kruskals algorithm until only 1 remains (mst created)
    int num_edges = 0;
    for (int i=0; i<2*w*h-(w+h); i++) {
        int n1 = edges[i].n1; int n2 = edges[i].n2;
        int id1 = node_regions[n1]; int id2 = node_regions[n2];
        // If nodes the edge connects aren't in the same region join them
        // This avoids loops and is what allows for a perfect maze
        if (id1 != id2) {
            join_regions(w*h, node_regions, id1, id2);
            num_regions--;
            new_edges[num_edges] = edges[i];
            num_edges++;
        }
        if (num_regions == 1) break;
    }
}

// Take the list of edges and use it to create a character representation of the maze
void convert(int w, int h, edge edges[(w*h)-1], char maze[2*h+1][2*w+2]) {
    // Fill maze array with # characters to represent walls
    for (int y=0; y<2*h+1; y++) {
        for (int x=0; x<2*w+2; x++) {
            maze[y][x] = '#';
        }
        maze[y][2*w+1] = '\0';
    }
    // Clear space for each node
    for (int i=0; i<w*h; i++) {
        int x = 2*(i % w) + 1;
        int y = 2*(i / w) + 1;
        maze[y][x] = ' ';
    }
    // Clear space for each edge
    for (int i=0; i<(w*h)-1; i++) {
        int n1 = edges[i].n1; int n2 = edges[i].n2;
        int x = (n1 % w) + (n2 % w) + 1;
        int y = (n1 / w) + (n2 / w) + 1;
        maze[y][x] = ' ';
    }
    // Make entrance and exit
    int x1 = 2 * (rand() % w) + 1;
    int x2 = 2 * (rand() % w) + 1;
    maze[0][x1] = ' ';
    maze[2*h][x2] = ' ';
}

// Display the maze
void display(int w, int h, char maze[2*h+1][2*w+2]) {
    for (int i=0; i<2*h+1; i++) {
        printf("%s\n", maze[i]);
    }
}

int get_size(char dimension[]) {
    char *ptr;
    bool valid = false;
    char input[16];
    while (!valid) {
        valid = true;
        printf("Please enter the %s:\n", dimension);
        fgets(input, 16, stdin);
        int length = strlen(input);
        for (int i=0; i<length-1 ; i++) {
            if (!isdigit(input[i])) valid = false;
        }
    }
    return strtol(input, &ptr, 10);
}

// Generate and display a maze of specified size
void gen_maze() {
    int w = 0;
    int h = 0;
    // Make sure width and height are valid ranges
    while (w < 2 || h < 2 || w > 100 || h > 100) {
        printf("\nPlease enter a width and height in range (2-100).\n\n");
        w = get_size("width");
        h = get_size("height");
    }
    edge edges[2*w*h-(w+h)];
    edge new_edges[(w*h)-1];
    char maze[2*h+1][2*w+2];
    gen_edge_list(w, h, edges);
    mst(w, h, edges, new_edges);
    convert(w, h, new_edges, maze);
    display(w, h, maze);
}

int main(int n, char *args[n]) {
    srand(time(NULL));
    char input[16];
    // Main menu UI + input
    while (strcmp(input, "2\n")) {
        printf("\n 1. Generate a new maze\n 2. Quit\n\n");
        fgets(input, 16, stdin);
        if (strcmp(input, "1\n") == 0) gen_maze();
    }
    return 0;
}
