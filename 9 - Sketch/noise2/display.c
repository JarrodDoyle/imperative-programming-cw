#include "display.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Set safe mode to true for environments with poor OpenGL support.
// Set it to false otherwise for greater efficiency.
enum { safe = true };

struct display {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

// If SDL fails, print the SDL error message, and stop the program.
static void fail() {
    fprintf(stderr, "Error: %s\n", SDL_GetError());
    SDL_Quit();
    exit(1);
}

// Check return values from SDL functions (Integer and Pointer versions).  These
// make error handling less intrusive.
static int I(int n) { if (n < 0) fail(); return n; }
static void *P(void *p) { if (p == NULL) fail(); return p; }

// Create a renderer - in software for safety or hardware for efficiency.
static void createRenderer(display *d) {
    if (safe) {
        SDL_Surface *surface = P(SDL_GetWindowSurface(d->window));
        d->renderer = P(SDL_CreateSoftwareRenderer(surface));
    }
    else d->renderer = P(SDL_CreateRenderer(d->window, -1, 0));
}

// Make recent changes appear on screen.
void show(display *d) {
    if (safe) I(SDL_UpdateWindowSurface(d->window));
    else SDL_RenderPresent(d->renderer);
}

// Create a new display object.
display *newDisplay(char *title, int width, int height) {
    setbuf(stdout, NULL);
    display *d = malloc(sizeof(display));
    I(SDL_Init(SDL_INIT_VIDEO));
    int x = SDL_WINDOWPOS_UNDEFINED, y = SDL_WINDOWPOS_UNDEFINED;
    d->window = P(SDL_CreateWindow(title, x, y, width, height, 0));
    createRenderer(d);
    I(SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255));
    I(SDL_RenderClear(d->renderer));
    show(d);
    I(SDL_SetRenderDrawColor(d->renderer, 0, 0, 0, 255));
    return d;
}

void resize(display *d, int width, int height){
    SDL_SetWindowSize(d->window, width, height);
    createRenderer(d);
}

void point(display *d, int x, int y) {
    I(SDL_RenderDrawPoint(d->renderer, x, y));
}

void colour(display *d, int rgb) {
    Uint8 r = (rgb >> 16) & 0xFF;
    Uint8 g = (rgb >> 8) & 0xFF;
    Uint8 b = (rgb ) & 0xFF;
    I(SDL_SetRenderDrawColor(d->renderer, r, g, b, 255));
}

void clear(display *d) {
    Uint8 r, g, b, a;
    I(SDL_GetRenderDrawColor(d->renderer, &r, &g, &b, &a));
    I(SDL_SetRenderDrawColor(d->renderer, 255, 255, 255, 255));
    I(SDL_RenderClear(d->renderer));
    show(d);
    I(SDL_SetRenderDrawColor(d->renderer, r, g, b, a));
}


void end(display *d) {
    SDL_Quit();
    free(d);
}
