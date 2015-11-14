#include <pebble.h>

#define BACKGROUND_CHANGE_ENABLED     1
#define DATE_ENABLED   								2

void create_background(Window *window);
void rebuild_background(void);
void destroy_background(void);