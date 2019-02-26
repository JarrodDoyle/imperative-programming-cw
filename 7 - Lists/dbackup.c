#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct c_rules{
    int birth_limit;
    int death_limit;
    int chance_to_live;
};
typedef struct c_rules c_rules;

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
void convert_maze(int w, int h, edge edges[(w*h)-1], bool maze[2*h+1][2*w+1]) {
    // Clear space for each node
    for (int i=0; i<w*h; i++) {
        int x = 2*(i % w) + 1;
        int y = 2*(i / w) + 1;
        maze[y][x] = false;
    }
    // Clear space for each edge
    for (int i=0; i<(w*h)-1; i++) {
        int n1 = edges[i].n1; int n2 = edges[i].n2;
        int x = (n1 % w) + (n2 % w) + 1;
        int y = (n1 / w) + (n2 / w) + 1;
        maze[y][x] = false;
    }
}

void attempt_room_placement(int w, int h, bool dungeon[h][w]){
    int width = ((3 + (rand() % 4)) / 2) * 2 + 1; // hardcoded max room sizes :/
    int height = ((3 + (rand() % 4)) / 2) * 2 + 1; // ""
    int x = 0; int y = 0;
    while (x % 2 == 0 || y % 2 == 0){
        x = rand() % w;
        y = rand() % h;
    }
    bool valid_room = true;
    for (int i=y; i<y+height; i++){
        for (int j=x; j<x+width; j++){
            if (i >= h - 1 || j >= w - 1) valid_room = false;
            else if (dungeon[i][j] == false) valid_room = false;
            if (!valid_room) break;
        }
        if (!valid_room) break;

    }
    if (valid_room){
        for (int i=y; i<y+height; i++){
            for (int j=x; j<x+width; j++){
                dungeon[i][j] = false;
            }
        }
    }
}

void merge_arrays(int w, int h, bool arr1[h][w], bool arr2[h][w]){
    for (int i=0; i<h; i++){
        for (int j=0; j<w; j++){
            if (!arr2[i][j]) arr1[i][j] = false;
        }
    }
}

void remove_dead_ends(int w, int h, bool arr[h][w]){
    bool done = false;
    int exits;
    int nx; int ny;
    while (!done){
        done = true;
        for (int y=0; y<h; y++){
            for (int x=0; x<w; x++){
                if (!arr[y][x]){
                    exits = 0;
                    for (int dx=-1; dx<2; dx+=2){
                        nx = x + dx;
                        if (nx >= 0 && nx < w && !arr[y][nx]) exits++;
                    }
                    for (int dy=-1; dy<2; dy+=2){
                        ny = y + dy;
                        if (ny >= 0 && ny < h && !arr[ny][x]) exits++;
                    }
                    if (exits == 1){
                        arr[y][x] = true;
                        done = false;
                    }
                }
            }
        }
    }
}

// Checks the Moore neighbourhood of a given cell
int check_neighbourhood(int x, int y, int w, int h, bool arr[h][w]){
    int alive = 0;
    int nx;
    int ny;
    for (int dy=-1; dy<2; dy++){
        for (int dx=-1; dx<2; dx++){
            if (dx != 0 || dy != 0){
                nx = x + dx;
                ny = y + dy;
                if (arr[ny][nx]) alive++;
            }
        }
    }
    return alive;
}

void gen_start(int w, int h, bool arr[h][w]){
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            arr[y][x] = true;
        }
    }
}
void gen_c_start(c_rules *r, int w, int h, bool arr[h][w]){
    for (int y=1; y<h-1; y++){
        for (int x=1; x<w-1; x++){
            if ((rand() % 100) < r->chance_to_live) arr[y][x] = true;
            else arr[y][x] = false;
        }
    }
    for (int x=0; x<w; x++){
        arr[0][x] = true;
        arr[h-1][x] = true;
    }
    for (int y=1; y<h-1; y++){
        arr[y][0] = true;
        arr[y][w-1] = true;
    }
}

// Does a single step of a cellular automata simulation
void cellular_automata(c_rules *r, int w, int h, bool arr[h][w]){
    bool new[h][w];
    for (int y=1; y<h-1; y++){
        for (int x=1; x<w-1; x++){
            int num_alive = check_neighbourhood(x, y, w, h, arr);
            if (arr[y][x] && num_alive < r->death_limit){
                new[y][x] = false;
            }
            else if (!arr[y][x] && num_alive > r->birth_limit){
                new[y][x] = true;
            }
            else new[y][x] = arr[y][x];
        }
    }
    for (int y=1; y<h-1; y++){
        for (int x=1; x<w-1; x++){
            arr[y][x] = new[y][x];
        }
    }
}

void convert(int w, int h, bool arr[h][w], char dungeon[h][w+1]){
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){
            if (arr[y][x]) dungeon[y][x] = '#';
            else dungeon[y][x] = ' ';
        }
        dungeon[y][w] = '\0';
    }
}

void display_dungeon(int w, int h, char dungeon[h][w+1]){
    for (int y=0; y<h; y++){
        printf("%s\n", dungeon[y]);
    }
}

void generate_cellular_automata(int w, int h, char d[h][w+1], int b_lim, int d_lim, int live_chance){
    bool arr[h][w];
    c_rules *r = malloc(sizeof(c_rules));
    *r = (c_rules) {b_lim, d_lim, live_chance};
    gen_c_start(r, w, h, arr);
    for (int i=0; i<6; i++) cellular_automata(r, w, h, arr);
    convert(w, h, arr, d);
    free(r);
}

void generate_mazeroom(int w, int h, char d[2*h+1][2*w+2], int room_attempts){
    bool maze[2*h+1][2*w+1];
    bool rooms[2*h+1][2*w+1];
    gen_start(2*w+1, 2*h+1, maze);
    gen_start(2*w+1, 2*h+1, rooms);
    edge edges[2*w*h-(w+h)];
    edge new_edges[(w*h)-1];
    gen_edge_list(w, h, edges);
    mst(w, h, edges, new_edges);
    convert_maze(w, h, new_edges, maze);
    for (int i=0; i<room_attempts; i++) attempt_room_placement(2*w+1, 2*h+1, rooms);
    merge_arrays(2*w+1, 2*h+1, maze, rooms);
    remove_dead_ends(2*w+1, 2*h+1, maze);
    convert(2*w+1, 2*h+1, maze, d);
}

int main(int n, char *args[n]) {
    int w = 100;
    int h = 60;
    char dungeon[h][w+1];
    for (int i=0; i<10; i++){
        generate_cellular_automata(w, h, dungeon, 4, 3, 40);
        display_dungeon(w, h, dungeon);
    }
    // w=(w-1)/2;
    // h=(h-1)/2;
    // char dungeon2[2*h+1][2*w+2];
    // generate_mazeroom(w, h, dungeon2, 100);
    // display_dungeon(2*w+1, 2*h+1, dungeon2);
    return 0;
}
