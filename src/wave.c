#include <pebble.h>
#include "round.h"

Window *_window;

TextLayer *time_layer;

BitmapLayer *_background_layer;
GBitmap *_background_bitmap;

// Background
void create_background(Window *window){
	
	_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WAVE_BACKGROUND);
	_background_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
	
	bitmap_layer_set_bitmap(_background_layer, _background_bitmap);
	
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(_background_layer));
}

void destroy_background(void){
	gbitmap_destroy(_background_bitmap);
	bitmap_layer_destroy(_background_layer);
}

// Time
void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  static char buffer[] = "00:00";

  if(clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  text_layer_set_text(time_layer, buffer);
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void create_watch(Window *window){
	
// SUPPORT FOR COLOR AND B&W
#ifdef PBL_COLOR
	time_layer = text_layer_create(GRect(15, 108, 144, 50));
  	text_layer_set_text_color(time_layer, GColorDukeBlue);
	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
#else
	time_layer = text_layer_create(GRect(26, 108, 144, 50));
  	text_layer_set_text_color(time_layer, GColorBlack);
	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
#endif
	
	text_layer_set_background_color(time_layer, GColorClear);
  	text_layer_set_text(time_layer, "00:00");
	
  	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
	
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
	update_time();
}

void destroy_watch(void){
	text_layer_destroy(time_layer);
}

void handle_init(void) {
	// Create a window and text layer
	_window = window_create();
	
#if defined(PBL_ROUND)
	create_round_time(_window);
#else	
	create_background(_window);
	create_watch(_window);
#endif
	
	// Push the window
	window_stack_push(_window, true);
}


void handle_deinit(void) {

#if defined(PBL_ROUND)
	destroy_round_time();
#else	
	destroy_background();
	destroy_watch();
#endif
	
	// Destroy the window
	window_destroy(_window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
