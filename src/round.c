#include <pebble.h>
#include "round.h"
	

GBitmap *_world_image;
BitmapLayer *_world_layer;


void create_world(Window *window){
	Layer *window_layer = window_get_root_layer(window);
  	GRect bounds = layer_get_bounds(window_layer);
	
	_world_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_WAVE_BACKGROUND);
	_world_layer = bitmap_layer_create(bounds);
	
	bitmap_layer_set_bitmap(_world_layer, _world_image);
	
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(_world_layer));
}

void destroy_world(){
	gbitmap_destroy(_world_image);
	bitmap_layer_destroy(_world_layer);
}

void create_round_time(Window *window){
	create_world(window);
}

void destroy_round_time(){
	destroy_world();
}