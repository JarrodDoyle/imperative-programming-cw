#include "display.h"

// State struct to keep track of the pen state
struct state;
typedef struct state state;

// Initialises the pen state
state *init_state();

// Update the pen state, moves the pen according to dx and dy
void update_state(state *s);

// Read and draw specified sketch file
void draw_sketch(char filename[], state *s, display *d);

// Do the operation defined in passed byte
void do_operation(unsigned char b, state *s, display *d);

// Returns the opcode of a byte
int get_opcode(unsigned char b);

// Returns the operand of a byte
int get_operand(unsigned char b);

// Toggle whether the pen is up or down
void pen_op(state *s);

// Updates dx of state struct
void dx_op(state *s, int dx);

// Updates dy of state struct and draws a line
void dy_op(state *s, int dy, display *d);

// Make graphics call
void draw_line(state *s, display *d);
