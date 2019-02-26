#include "perlin.h"
#include "display.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Map colours
enum {
    deepocean=0x1631da,
    ocean=0x115fff,
    beach=0xe3ed7d,
    plain=0x53ee26,
    forest=0x0b6622,
    rocky=0x936706,
    mountain=0x594812,
    snow=0xfffdff
};

// Return the biome/colour at given elevation value
int point_colour(float elevation){
    if (elevation < 0.35) return deepocean;
    else if (elevation < 0.4) return ocean;
    else if (elevation < 0.425) return beach;
    else if (elevation < 0.5) return plain;
    else if (elevation < 0.55) return forest;
    else if (elevation < 0.6) return rocky;
    else if (elevation < 0.65) return mountain;
    else return snow;
}

// Return noise value using appropriate frequency and amplitude
// Also changes frequency and amplitude according to the lacunarity and persistence
// in preperation for the next octave
float octave(float x, float y, float *hz, float *amp, float per, float lac, int h[512]){
    float e = perlin2d(*hz * x, *hz * y, h) * *amp;
    // Adjust frequency and amplitude for next octave
    *hz *= lac;
    *amp *= per;
    return e;
}

// Generates a 2d array of values representing the height at each point in the map
void gen_hmap(int w, int h, float hmap[h][w], float bhz, float per, float lac, int oct, int seed){
    int hash[512];
    gen_hash(hash, seed);
    // Loop over every point of the map
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){
            // Scale x and y so the map looks the same at different resolutions
            float nx = (float)x / (float)w;
            float ny = (float)y / (float)h;
            // Reset frequency and amplitude for each point to account for changes
            // when calculating octaves
            float hz = bhz;
            float amp = 1;
            // Sum octaves together and keep track of the total amplitude so that
            // the elevation can be brought back to range [0, 1]
            float total = 0; float total_amp = 0;
            for (int i=0; i<oct; i++){
                total_amp += amp;
                total += octave(nx, ny, &hz, &amp, per, lac, hash);
            }
            hmap[y][x] = total / total_amp;
        }
    }
}

// Render the map using SDL
void display_map(int w, int h, float hmap[h][w], display *d){
    // Loop through each point of the map and colour the appropriate pixel
    for (int y=0; y<h; y++){
        for (int x=0; x<w; x++){
            colour(d, point_colour(hmap[y][x]));
            point(d, x, y);
        }
    }
    show(d);
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

// Return an integer representation of input.
float get_float(char dimension[]) {
    char *ptr;
    bool valid = false;
    char input[16];
    // While the input isn't valid, keep asking for input.
    while (!valid) {
        valid = true;
        printf("Please enter the %s:\n", dimension);
        fgets(input, 16, stdin);
        if (strtof(input, &ptr) == (float)0) valid = false;
    }
    return strtof(input, &ptr);
}

// Main loop
int main(int n, char *args[n]){
    char input[16];
    int oct, w, h;
    float hz, per, lac;
    display *d = newDisplay("Perlin Noise Map", 200, 200);
    // Main menu and UI
    while (strcmp(input, "2\n")){
        printf("\n 1. Generate map\n 2. Quit\n\n");
        fgets(input, 16, stdin);
        // User wants to generate a map
        if (strcmp(input, "1\n") == 0){
            int seed = time(NULL);
            w = 0; h = 0; oct = 0; hz = 0; per = 0; lac = 0;
            while (w < 256 || w > 1024 || h < 256 || h > 768 ||
                    oct < 1 || oct > 10 || hz <= 0 || hz > 20 ||
                    per <= 0 || per > 1 || lac < 1 || lac > 4){
                w = get_integer("map width (256 - 1024)");
                h = get_integer("map height (256 - 768)");
                oct = get_integer("number of octaves (1 - 10)");
                hz = get_float("frequency (0 < f <= 20)");
                per = get_float("persistence (0 < p <= 1)");
                lac = get_float("lacunarity (1 <= l <= 4)");
                printf("\n");
            }
            resize(d, w, h);
            float hmap[h][w];
            gen_hmap(w, h, hmap, hz, per, lac, oct, seed);
            display_map(w, h, hmap, d);
        }
    }
    end(d);
}
