#include <pebble.h>

#define BACKGROUND_CHANGE_ENABLED     1
#define DATE_ENABLED   								2
#define BT_VIBRATE_ENABLED						3

void create_background(Window *window);
void choose_background(bool bt_connect);
void rebuild_background(void);
void destroy_background(void);