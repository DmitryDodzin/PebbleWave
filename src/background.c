#include "background.h"

BitmapLayer *_background_layer;
GBitmap *_background_bitmap;


void choose_background(bool bt_connected){
	if(_background_bitmap){
		gbitmap_destroy(_background_bitmap);
	}

	if(!persist_read_bool(BACKGROUND_CHANGE_ENABLED)){
		bt_connected = true; // Override the bt connection flag if color change is dissabled
		APP_LOG(APP_LOG_LEVEL_DEBUG, "BT status forced on");
	}

	APP_LOG(APP_LOG_LEVEL_DEBUG, "Rebuilding the background (bluetooth -> %d)", bt_connected);

	if(bt_connected){
		_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WAVE_BACKGROUND);
	} else {
		_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_DES_WAVE_BACKGROUND);
	}
	
	bitmap_layer_set_bitmap(_background_layer, _background_bitmap);
}

void rebuild_background(void){
	#ifdef PBL_SDK_2
		choose_background(bluetooth_connection_service_peek());
	#elif PBL_SDK_3
		choose_background(connection_service_peek_pebble_app_connection());
	#endif
}

// Background
void create_background(Window *window){
	Layer *window_layer = window_get_root_layer(window);

	_background_layer = bitmap_layer_create(layer_get_bounds(window_layer));
	
#ifdef PBL_SDK_2
	choose_background(bluetooth_connection_service_peek());
#elif PBL_SDK_3
	choose_background(connection_service_peek_pebble_app_connection());
#endif
	
	bitmap_layer_set_bitmap(_background_layer, _background_bitmap);
	
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(_background_layer));

#ifdef PBL_SDK_2
	bluetooth_connection_service_subscribe(choose_background);
#elif PBL_SDK_3
	connection_service_subscribe((ConnectionHandlers) {
		.pebble_app_connection_handler = choose_background
	});
#endif
}

void destroy_background(void){
	gbitmap_destroy(_background_bitmap);
	bitmap_layer_destroy(_background_layer);
}