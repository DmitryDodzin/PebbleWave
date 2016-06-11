#include "battery.h"

BitmapLayer *_battery_indicator_layer;
GBitmap *_battery_indicator_bitmap;

GBitmap *_battery_indicatior_low_bitmap;
GBitmap *_battery_indicatior_charging_bitmap;
GBitmap *_battery_indicator_empty_bitmap;


void create_battry_indicator(Window *window){
	_battery_indicator_layer = bitmap_layer_create(GRect(10, 10, 124, 30));

	bitmap_layer_set_alignment(_battery_indicator_layer, GAlignTopRight);


	_battery_indicator_empty_bitmap = gbitmap_create_blank(GSize(16, 7), GBitmapFormat1Bit);

	_battery_indicator_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATTERY_INDICATORS);

	_battery_indicatior_low_bitmap = gbitmap_create_as_sub_bitmap(_battery_indicator_bitmap, GRect(0, 0, 16, 7));
	_battery_indicatior_charging_bitmap = gbitmap_create_as_sub_bitmap(_battery_indicator_bitmap, GRect(0, 7, 16, 7));

	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(_battery_indicator_layer));
}

void update_battry_indicator(BatteryChargeState chargeState){
	if(persist_exists(BATTERY_INDICATOR_ENABLED) && persist_read_bool(BATTERY_INDICATOR_ENABLED)){
		if(chargeState.is_charging){
			bitmap_layer_set_compositing_mode(_battery_indicator_layer, GCompOpAssign);
			bitmap_layer_set_bitmap(_battery_indicator_layer, _battery_indicatior_charging_bitmap);
		} else if (chargeState.charge_percent < 20){
			bitmap_layer_set_compositing_mode(_battery_indicator_layer, GCompOpAssign);
			bitmap_layer_set_bitmap(_battery_indicator_layer, _battery_indicatior_low_bitmap);
		} else {
			clear_battry_indicator();
		}
	}
	else {
		clear_battry_indicator();
	}
}

void clear_battry_indicator(void){
	bitmap_layer_set_compositing_mode(_battery_indicator_layer, GCompOpClear);
	bitmap_layer_set_bitmap(_battery_indicator_layer, _battery_indicator_empty_bitmap);
}

void destroy_battry_indicator(void){
	gbitmap_destroy(_battery_indicator_bitmap);
	gbitmap_destroy(_battery_indicator_empty_bitmap);
	gbitmap_destroy(_battery_indicatior_low_bitmap);
	gbitmap_destroy(_battery_indicatior_charging_bitmap);
	bitmap_layer_destroy(_battery_indicator_layer);
}