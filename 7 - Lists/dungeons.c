#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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

// Attempts to place a specified amount of rooms
void attempt_room_placement(int w, int h, bool dungeon[h][w]){
    int width = ((3 + (rand() % 4)) / 2) * 2 + 1;
    int height = ((3 + (rand() % 4)) / 2) * 2 + 1;
    int x = 0; int y = 0;
    // Generate x and y coordinates for top left corner of the room
    // Coords must be odd due to maze generation, so repeat until they are odd
    while (x % 2 == 0 || y % 2 == 0){
        x = rand() % w;
        y = rand() % h;
    }
    // Check if the room intersects with an existing room. Discard if it does.
    bool valid_room = true;
    for (int i=y; i<y+height; i++){
        for (int j=x; j<x+width; j++){
            if (i >= h - 1 || j >= w - 1) valid_room = false;
            else if (dungeon[i][j] == false) valid_room = false;
            if (!valid_room) break;
        }
        if (!valid_room) break;

    }
    // If the room hasn't been discarded, build it.
    if (valid_room){
        for (int i=y; i<y+height; i++){
            for (int j=x; j<x+width; j++){
                dungeon[i][j] = false;
            }
        }
    }
}

// False points in arr2 replace the same point in arr1
// Used to merge rooms onto the generated maze.
void merge_arrays(int w, int h, bool arr1[h][w], bool arr2[h][w]){
    for (int i=0; i<h; i++){
        for (int j=0; j<w; j++){
            if (!arr2[i][j]) arr1[i][j] = false;
        }
    }
}

// Removes dead end points from the maze. Done after merging with rooms.
// This is done to create winding corridoors that don't have dead ends.
void remove_dead_ends(int w, int h, bool arr[h][w]){
    bool done = false;
    int exits;
    int nx; int ny;
    // Repeat iterating over the arr until a pass is made without any changes
    while (!done){
        done = true;
        for (int y=0; y<h; y++){
            for (int x=0; x<w; x++){
                // If the point is a floor, check how many floors are arount it
                // If there is only one exit, the point is a dead end.
                // Dead ends become walls (true).
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
            // If dx and dy are 0 we would be checking the cell who's neighbourhood we are finding.
            // We don't want this obviously, so ignore this case.
            if (dx != 0 || dy != 0){
                nx = x + dx;
                ny = y + dy;
                if (arr[ny][nx]) alive++;
            }
        }
    }
    return alive;
}

// Fills a boolean array with true values (represents walls)
void gen_start(int w, int h, bool arr[h][w]){
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            arr[y][x] = true;
        }
    }
}

// Fills a boolean array to be used for a cellular automata
// Points are randomly true or false according to given chance to start alive.
void gen_c_start(c_rules *r, int w, int h, bool arr[h][w]){
    // Only randomise the points not on the outside of the grid.
    // This is because we want to make sure there is a solid wall on the outside.
    for (int y=1; y<h-1; y++){
        for (int x=1; x<w-1; x++){
            if ((rand() % 100) < r->chance_to_live) arr[y][x] = true;
            else arr[y][x] = false;
        }
    }
    // Sets the outsides to be walls.
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
    // Creates a new array to hold the result of the step
    // If you don't do this you end up with updated cells affecting cells that haven't updated yes
    // This isn't what you want. All cells should update "simultaneously".
    bool new[h][w];
    for (int y=1; y<h-1; y++){
        for (int x=1; x<w-1; x++){
            int num_alive = check_neighbourhood(x, y, w, h, arr);
            // Death by starvation
            if (arr[y][x] && num_alive < r->death_limit){
                new[y][x] = false;
            }
            // Birth due to high population
            else if (!arr[y][x] && num_alive > r->birth_limit){
                new[y][x] = true;
            }
            // If neither cases above are true, state hasn't changed so copy from original array.
            else new[y][x] = arr[y][x];
        }
    }
    // Copy new array into the passed array.
    for (int y=1; y<h-1; y++){
        for (int x=1; x<w-1; x++){
            arr[y][x] = new[y][x];
        }
    }
}

// Performs a recursive floodfill using integers
int floodfill_int(int x, int y, int w, int h, int arr[h][w], int target, int replacement, int size){
    if (target != replacement && arr[y][x] == target){
        arr[y][x] = replacement;
        size++;
        // Call recursively to the 4 cardinal directions from current point.
        size = floodfill_int(x, y-1, w, h, arr, target, replacement, size);
        size = floodfill_int(x, y+1, w, h, arr, target, replacement, size);
        size = floodfill_int(x-1, y, w, h, arr, target, replacement, size);
        size = floodfill_int(x+1, y, w, h, arr, target, replacement, size);
    }
    // Return the size of the area filled
    return size;
}

// Performs a recursive floodfill using booleans
void floodfill_bool(int x, int y, int w, int h, bool arr[h][w], bool target, bool replacement){
    if (target != replacement && arr[y][x] == target){
        arr[y][x] = replacement;
        // Call recursively to the 4 cardinal directions from current point.
        floodfill_bool(x, y-1, w, h, arr, target, replacement);
        floodfill_bool(x, y+1, w, h, arr, target, replacement);
        floodfill_bool(x-1, y, w, h, arr, target, replacement);
        floodfill_bool(x+1, y, w, h, arr, target, replacement);
    }
}

// Connect a specified point to the center of the array.
void connect_to_center(int x, int y, int w, int h, bool arr[h][w]){
    int cy = h/2;
    int cx = w/2;
    int temp;
    // Reorder x/cx and y/cy if necessary
    if (x > cx){
        temp = cx;
        cx = x;
        x = temp;
    }
    if (y > cy){
        temp = cy;
        cy = y;
        y = temp;
    }
    // Randomly decide whether to do horizontal or vertical first
    if (rand() % 2 == 1){
        // Horizontal then vertical
        for (int i=x; i<cx+1; i++) arr[y][i] = false;
        for (int j=y; j<cy+1; j++) arr[j][cx] = false;
    }
    else {
        // Vertical then horizontal
        for (int j=y; j<cy+1; j++) arr[j][x] = false;
        for (int i=x; i<cx+1; i++) arr[cy][i] = false;
    }
}

// Connect regions of a cellular automata together.
void connect_regions(int w, int h, bool arr[h][w]){
    // Generate the base regions with 0 representing walls and 1 for floors.
    int reg[h][w];
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){
            if (arr[y][x]) reg[y][x] = 0;
            else reg[y][x] = 1;
        }
    }
    int size;
    int reg_id = 2;
    // Give each cave area it's own region ID.
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){
            if (reg[y][x] == 1){
                size = floodfill_int(x, y, w, h, reg, 1, reg_id, 0);
                // If the size of the region is less than 100 fill it in.
                if (size <= 100){
                    floodfill_bool(x, y, w, h, arr, false, true);
                    floodfill_int(x, y, w, h, reg, reg_id, 0, 0);
                }
                reg_id++;
            }
        }
    }
    // Connect all the regions together. This is done simply by connecting all
    // regions to the center of the map.
    reg_id = 2;
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){
            if (reg[y][x] >= reg_id){
                connect_to_center(x, y, w, h, arr);
                reg_id++;
            }
        }
    }
}

// Convert boolean representation of dungeon to character representation.
void convert(int w, int h, bool arr[h][w], char dungeon[h][w+1]){
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){
            // If the bool is true it's a wall, if false a floor.
            if (arr[y][x]) dungeon[y][x] = '#';
            else dungeon[y][x] = ' ';
        }
        // Place null char at the end of each row so they can be read as strings.
        dungeon[y][w] = '\0';
    }
}

// Display the character representation of the dungeon.
void display_dungeon(int w, int h, char dungeon[h][w+1]){
    for (int y=0; y<h; y++){
        // Each row ends with a null char so can be read as a complete string.
        printf("%s\n", dungeon[y]);
    }
}

// Completely generate a cellular automata cave dungeon.
void generate_cellular_automata(int w, int h, char d[h][w+1], int b_lim, int d_lim, int live_chance, int steps){
    bool arr[h][w];
    c_rules *r = malloc(sizeof(c_rules));
    *r = (c_rules) {b_lim, d_lim, live_chance};
    gen_c_start(r, w, h, arr);
    for (int i=0; i<steps; i++) cellular_automata(r, w, h, arr);
    connect_regions(w, h, arr);
    convert(w, h, arr, d);
    free(r);
}

// Completely generate a "mazes and rooms" dungeon.
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

// Return an integer representation of input.
int get_integer(char dimension[]) {
    char *ptr;
    bool valid = false;
    char input[16];
    // While the input isn't valid, keep asking for input.
    while (!valid) {
        valid = true;
        printf("Please enter the %s:\n", dimension);
        fgets(input, 16, stdin);
        int length = strlen(input);
        // Checks that all characters in input are digits
        for (int i=0; i<length-1 ; i++) {
            if (!isdigit(input[i])) valid = false;
        }
    }
    return strtol(input, &ptr, 10);
}

// Main loop, holds UI and input stuffs.
int main(int n, char *args[n]) {
    srand(time(NULL));
    char input[16];
    // Main menu UI + input
    while (strcmp(input, "3\n")) {
        printf("\n 1. Generate caves\n 2. Generate 'Rooms and Mazes'\n 3. Quit\n\n");
        fgets(input, 16, stdin);
        // Wants to generate a caves dungeon.
        if (strcmp(input, "1\n") == 0){
            // Lots of input validation.
            int w = 0;
            int h = 0;
            int b = 0;
            int d = 0;
            int c = -1;
            int s = 0;
            while (w < 50 || h < 50 || w > 100 || h > 100){
                printf("\nPlease enter a width and height in range (50-100).\n\n");
                w = get_integer("width");
                h = get_integer("height");
            }
            while (b < 1 || d < 1 || b > 8 || d > 8){
                printf("\nPlease enter a birth and death rate in range (1-8)\n\n");
                b = get_integer("birth limit");
                d = get_integer("death limit");
            }
            while (c < 0 || c > 100) c = get_integer("percentage chance to be alive at the start");
            while (s < 1 || s > 10){
                printf("\nPlease enter how many steps of the simulation should take place in range (1-10)\n\n");
                s = get_integer("simulation steps");
            }
            char cellular[h][w+1];
            generate_cellular_automata(w, h, cellular, b, d, c, s);
            display_dungeon(w, h, cellular);
        }
        // Wants to generate a rooms and mazes dungeon.
        else if (strcmp(input, "2\n") == 0){
            // Lots of input validation.
            int w = 0;
            int h = 0;
            int r = 0;
            while (w < 50 || h < 50 || w > 200 || h > 200){
                printf("\nPlease enter a width and height in range (50-200).\n\n");
                w = get_integer("width");
                h = get_integer("height");
            }
            w = (w - 1) / 2;
            h = (h - 1) / 2;
            while (r < 10 || r > 1000){
                printf("\nPlease enter the amout of room placement attemps in range (10-1000)\n\n");
                r = get_integer("number of room placement attempts");
            }
            char roomsnmazes[2*h+1][2*w+2];
            generate_mazeroom(w, h, roomsnmazes, r);
            display_dungeon(2*w+1, 2*h+1, roomsnmazes);
        }
    }
    return 0;
}
