#include <pebble.h>
#include <background.h>

#define BACKGROUND_CHANGE_ENABLED     1
#define DATE_ENABLED   								2

Window *_window;

TextLayer *time_layer;

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
  #ifdef PBL_ROUND
    time_layer = text_layer_create(GRect(0, 100, 180, 50));
    text_layer_set_text_color(time_layer, GColorBlack);
  #else
	  time_layer = text_layer_create(GRect(15, 108, 144, 50));
    text_layer_set_text_color(time_layer, GColorOxfordBlue);
  #endif
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

void inbox_received_handler(DictionaryIterator *iter, void *context) {

  Tuple *bg_change_t = dict_find(iter, BACKGROUND_CHANGE_ENABLED);
  Tuple *date_enable_t = dict_find(iter, DATE_ENABLED);

  bool bg_change = bg_change_t->value->int8;
  bool date_enable = date_enable_t->value->int8;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pebble: BACKGROUND_CHANGE_ENABLED -> %d", bg_change);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pebble: DATE_ENABLED %d", date_enable);

  persist_write_bool(BACKGROUND_CHANGE_ENABLED, bg_change);
  persist_write_bool(DATE_ENABLED, date_enable);
}

void handle_init(void) {
	_window = window_create();
	
	create_background(_window);
	create_watch(_window);
	
	window_stack_push(_window, true);

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}


void handle_deinit(void) {

	destroy_background();
	destroy_watch();
	
	window_destroy(_window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
