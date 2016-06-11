#pragma once
#include <pebble.h>

#define BACKGROUND_CHANGE_ENABLED			1
#define DATE_ENABLED									2
#define BT_VIBRATE_ENABLED						3
#define BATTERY_INDICATOR_ENABLED			4

void create_battry_indicator(Window *window);
void update_battry_indicator(BatteryChargeState chargeState);
void clear_battry_indicator(void);
void destroy_battry_indicator(void);