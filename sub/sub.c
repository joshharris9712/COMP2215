#include "sub.h"

int main(){
	
	
	
	init_lcd();
    set_frame_rate_hz(61);
    set_orientation(West);
	
	
	_delay_ms(1000);
	
	display_sprite(sub_sprite, 5, 5, YELLOW);
	
}