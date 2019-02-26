#include "sketch.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

struct state{
    bool pen_down;
    int x;
    int y;
    int dx;
    int dy;
};

enum{dx = 0, dy = 1, what = 2, pen = 3};

state *init_state(){
    state *s = malloc(sizeof(state));
    *s = (state) {false, 0, 0, 0, 0};
    return s;
}

void update_state(state *s){
    s->x += s->dx;
    s->y += s->dy;
    s->dx = 0;
    s->dy = 0;
}

void draw_sketch(char filename[], state *s, display *d){
    FILE *in = fopen(filename, "rb");
    unsigned char b = fgetc(in);
    while (!feof(in)){
        do_operation(b, s, d);
        b = fgetc(in);
    }
    fclose(in);
    end(d);
}

void do_operation(unsigned char b, state *s, display *d){
    int oc = get_opcode(b);
    int or = get_operand(b);
    if (oc == dx) dx_op(s, or);
    else if (oc == dy) dy_op(s, or, d);
    else if (oc == pen) pen_op(s);
}

int get_opcode(unsigned char b){
    return b >> 6;
}

int get_operand(unsigned char b){
    int or = b & ((1 << 6) - 1);
    return (or >> 5 == 1) ? (~0U << 6) | or : or;
}

void pen_op(state *s){
    s->pen_down = !s->pen_down;
}

void dx_op(state *s, int dx){
    s->dx = dx;
}

void dy_op(state *s, int dy, display *d){
    s->dy = dy;
    if (s->pen_down) draw_line(s, d);
    update_state(s);
}

void draw_line(state *s, display *d){
    int x1 = s->x;
    int y1 = s->y;
    int x2 = x1 + s->dx;
    int y2 = y1 + s->dy;
    line(d, x1, y1, x2, y2);
}

int main(int n, char *args[n]) {
    if (n != 2) { fprintf(stdout, "Use ./sketch filename\n"); exit(1); }
    display *d = newDisplay(args[1], 200, 200);
    state *s = init_state();
    draw_sketch(args[1], s, d);
    free(s);
    free(d);
}
