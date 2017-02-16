#include "sub.h"

volatile redraw_flag = 0, tick_count=0;
float direction = 0.0;
uint8_t player_x = 50, player_y = 50;
rectangle player_box;

int main(){
	
	init();
	
	display_sprite(sub_sprite, player_x, player_y, YELLOW);
	
	display_sprite(jelly_sprite, 40, 120, BLUE);
	
	display_sprite(jelly_sprite, 120, 160, GREEN);
	
	display_sprite(bomb_sprite, 200, 200, PURPLE);
	
	display_sprite(pwr_up_sprite, 20, 20, RED);
	
	sei();
	
	while(1){
		
		if(redraw_flag){
			fill_rectangle(player_box, BLACK);
			player_box.left=player_x;
			player_box.right=player_x+8;
			player_box.top=player_y;
			player_box.bottom=player_y+18;
			display_sprite(sub_sprite, player_x, player_y, YELLOW);
			redraw_flag=0;
		}
		_delay_ms(10);

	}

	
}

void init(){
	
	DDRB  |=  _BV(PB7);
	PORTB &= ~_BV(PB7);
	
	CLKPR = (1 << CLKPCE);
	CLKPR = 1;
	
	DDRE &= ~_BV(PE4);	/* ROTB pin in */
	PORTE |= _BV(PE4);	/* ROTB off */
	
	DDRE &= ~_BV(PE5);	/* ROTA pin in */
	PORTE |= _BV(PE5);	/* ROTA off */
	
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);   /* F_CPU / 64 */

	OCR0A = (uint8_t)(F_CPU / 64 / 1000 - 0.5);
	
	TIMSK0 |= _BV(OCIE0A);
	
	
	init_lcd();
    set_frame_rate_hz(61);
    set_orientation(West);
	
	player_box.left=player_x;
	player_box.right=player_x+8;
	player_box.top=player_y;
	player_box.bottom=player_y+8;
	
}

ISR( TIMER0_COMPA_vect ) {
     static int8_t last;
     int8_t new, diff;
     uint8_t wheel;
	 

     /*
        Scan rotary encoder
        ===================
        This is adapted from Peter Dannegger's code available at:
        http://www.mikrocontroller.net/articles/Drehgeber
     */

     wheel = PINE;
     new = 0;
     if( wheel  & _BV(PE4) ) new = 3;
     if( wheel  & _BV(PE5) )
	 new ^= 1;		        	/* convert gray to binary */
     diff = last - new;			/* difference last - new  */
     if( diff & 1 ){			/* bit 0 = value (1) */
	     last = new;		       	/* store new as next last  */
	     delta += (diff & 2) - 1;	/* bit 1 = direction (+/-) */
     }
	 
	 if(tick_count>10){
		tick_count=0;
		player_x+=round(3*sin((double) delta/10));
		player_y+=round(3*cos((double) delta/10));
		if(player_x<0){
			player_x=0;
		}
		if(player_y<0){
			player_y=0;
		}
		redraw_flag=1;
	 }else{
		++tick_count;
	 }

}

	
	
