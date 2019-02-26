// A display structure needs to be created by calling newDisplay, and then
// needs to be passed to each sketching function.
struct display;
typedef struct display display;

// Create a display object representing a plain white window of a given size.
display *newDisplay(char *title, int width, int height);

void resize(display *d, int width, int height);

// Draw a point on the display.
void point(display *d, int x, int y);

// Show changes on display.
void show(display *d);

// Change the drawing colour.
void colour(display *d, int rgb);

// Clear the display to white.
void clear(display *d);

// Hold the display for a few seconds, then shut down.
void end(display *d);
