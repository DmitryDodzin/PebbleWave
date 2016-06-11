#include <pebble.h>
#include <background.h>
#include <battery.h>

#define BACKGROUND_CHANGE_ENABLED     1
#define DATE_ENABLED   								2
#define BT_VIBRATE_ENABLED            3
#define BATTERY_INDICATOR_ENABLED     4

Window *_window;

TextLayer *time_layer;
TextLayer *date_layer;

// Time
void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  static char date_buffer[16];
  static char time_buffer[] = "00:00";

  if(clock_is_24h_style() == true) {
    strftime(time_buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(time_buffer, sizeof("00:00"), "%I:%M", tick_time);
  }

  if(persist_exists(DATE_ENABLED)){
    if(persist_read_bool(DATE_ENABLED)){
      strftime(date_buffer, sizeof(date_buffer), "%a %d %b", tick_time);
    }
    else {
      memset(date_buffer, 0, 16);
    }
  }

  text_layer_set_text(time_layer, time_buffer);
  text_layer_set_text(date_layer, date_buffer);
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

    date_layer = text_layer_create(GRect(0, 140, 180, 50));
    text_layer_set_text_color(date_layer, GColorBlack);
  #else
	  time_layer = text_layer_create(GRect(15, 108, 144, 50));
    text_layer_set_text_color(time_layer, GColorOxfordBlue);

    date_layer = text_layer_create(GRect(40, 96, 80, 20));
    text_layer_set_text_color(date_layer, GColorOxfordBlue);
  #endif
	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
#else
	time_layer = text_layer_create(GRect(26, 108, 144, 50));
  text_layer_set_text_color(time_layer, GColorBlack);

  date_layer = text_layer_create(GRect(26, 138, 144, 50));
  text_layer_set_text_color(date_layer, GColorBlack);

	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
  text_layer_set_font(date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
#endif
	
	text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_background_color(date_layer, GColorClear);

  text_layer_set_text(time_layer, "00:00");
  text_layer_set_text(date_layer, "");
	
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(date_layer, GTextAlignmentCenter);
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_layer));
	
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
	update_time();
}

void destroy_watch(void){
	text_layer_destroy(time_layer);
}

void refresh_after_config(){
  rebuild_background();
  update_time();
  update_battry_indicator(battery_state_service_peek());
}

void handle_bt_change(bool bt_connectd){
  choose_background(bt_connectd);

  if(persist_exists(BT_VIBRATE_ENABLED) && persist_read_bool(BT_VIBRATE_ENABLED)){
    vibes_short_pulse();
    light_enable_interaction();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Vibed for bt disconnect");
  }
}

void inbox_received_handler(DictionaryIterator *iter, void *context) {

  Tuple *bg_change_t = dict_find(iter, BACKGROUND_CHANGE_ENABLED);
  Tuple *date_enable_t = dict_find(iter, DATE_ENABLED);
  Tuple *bt_vibrate_t = dict_find(iter, BT_VIBRATE_ENABLED);
  Tuple *battery_indic_t = dict_find(iter, BATTERY_INDICATOR_ENABLED);

  bool bg_change = bg_change_t->value->int8;
  bool date_enable = date_enable_t->value->int8;
  bool bt_vibrate = bt_vibrate_t->value->int8;
  bool battery_indic = battery_indic_t->value->int8;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pebble: BACKGROUND_CHANGE_ENABLED -> %d", bg_change);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pebble: DATE_ENABLED %d", date_enable);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pebble: BT_VIBRATE_ENABLED %d", bt_vibrate);  

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Pebble: BATTERY_INDICATOR_ENABLED %d", battery_indic);  

  persist_write_bool(BACKGROUND_CHANGE_ENABLED, bg_change);
  persist_write_bool(DATE_ENABLED, date_enable);
  persist_write_bool(BT_VIBRATE_ENABLED, bt_vibrate);
  persist_write_bool(BATTERY_INDICATOR_ENABLED, battery_indic);

  refresh_after_config();
}

void handle_init(void) {
	_window = window_create();
  
  create_background(_window);
  create_watch(_window);
  create_battry_indicator(_window);

  #ifdef PBL_SDK_2
    bluetooth_connection_service_subscribe(handle_bt_change);
  #elif PBL_SDK_3
    connection_service_subscribe((ConnectionHandlers) {
      .pebble_app_connection_handler = handle_bt_change
    });
  #endif

  battery_state_service_subscribe(update_battry_indicator);
  
  window_stack_push(_window, true);
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}


void handle_deinit(void) {
  battery_state_service_unsubscribe();

  destroy_background();
  destroy_watch();
  destroy_battry_indicator();
  
  window_destroy(_window);
}

int main(void) {
	handle_init();
	app_event_loop();
	handle_deinit();
}
