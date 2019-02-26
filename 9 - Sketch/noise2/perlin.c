#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// Generates a list of 256 integers in range [0, 255] inclusive and repeats it
void gen_hash(int h[512], int seed){
    // Set the seed for random functions
    srand(seed);
    // Fill array with integers 0 through 255 inclusive
    int hashed[256];
    for (int i=0; i<256; i++) hashed[i] = i;
    // Shuffle array
    for (int i=0; i<255; i++){
        int j = i + (rand() % (256 - i));
        int temp = hashed[j];
        hashed[j] = hashed[i];
        hashed[i] = temp;
    }
    // Repeat array to account for overflow later
    for (int i=0; i<512; i++) h[i] = hashed[i % 256];
}

// Computes the dot product of a gradient vector and position vector
float dot2d(int i, float x, float y){
    // 8 potential gradient directions
    int grads[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
    float dx = x - (int)x;
    float dy = y - (int)y;
    float d = grads[i][0] * dx + grads[i][1] * dy;
    return d;
}

// Fade function defined by Ken Perlin in his SIGGRAPH 2002 paper on Improved
// Perlin Noise as 6t^5 - 15t^4 + 10t^3
float fade(float t){
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Simple weighted linear interpolation between two floats
float lerp(float a, float b, float w){
    return a + w * (b - a);
}

// Hash function
int hfunc(int x, int y, int h[512]){
    // This is where overflow would happen if hash array wasn't repeated
    return h[h[x] + y];
}

// Return the noise value at specified point with given seed
float perlin2d(float x, float y, int h[512], int seed){
    // Unit square containing point (x, y)
    int xi = (int)floorf(x);
    int yi = (int)floorf(y);
    // Position of (x, y) relative to unit square
    float fx = x - xi;
    float fy = y - yi;
    // Binds xi and yi to [0, 255] inclusive
    xi &= 255; yi &= 255;
    // Calculate dot products between gradients at unit square corners
    float aa = dot2d(hfunc(xi, yi, h) & 7, fx, fy);
    float ba = dot2d(hfunc(xi+1, yi, h) & 7, fx - 1, fy);
    float ab = dot2d(hfunc(xi, yi+1, h) & 7, fx, fy - 1);
    float bb = dot2d(hfunc(xi+1, yi+1, h) & 7, fx - 1, fy - 1);
    // Linear interpolation of dot products, uses faded values for weights
    // to ensure a continuous gradient with no artifacts
    float u = fade(fx);
    float v = fade(fy);
    float e = lerp(lerp(aa, ba, u), lerp(ab, bb, u), v);
    // Bind noise value to [0, 1]
    return (e + 1) / 2;
}
