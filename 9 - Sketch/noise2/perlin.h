// Generates a list of 256 integers in range [0, 255] inclusive and repeats it
// for later use with hfunc()
void gen_hash(int h[512], int seed);

// Computes the dot product of a gradient vector and position vector
float dot2d(int i, float x, float y);

// Return faded value according to improved fade function defined by Ken Perlin
float fade(float t);

// Weighted linear interpolation
float lerp(float a, float b, float w);

// Returns hashed value that corresponds to (x, y)
float hfunc(int x, int y, int h[512]);

// Returns a noise value at point (x, y)
float perlin2d(float x, float y, int h[512]);
