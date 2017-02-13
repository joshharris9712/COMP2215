#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd/lcd.h"

#define SWN     PC2
#define SWE     PC3
#define SWS     PC4
#define SWW     PC5
#define SWC     PE7
#define ROTA	PE5
#define ROTB	PE4
#define COMPASS_SWITCHES (_BV(SWW)|_BV(SWS)|_BV(SWE)|_BV(SWN))

void redraw();
int leftPressed();
int rightPressed();
char str[51]="A";
static int charPointer=0, changeFlag = 1, leftStep=0, rightStep=0;
int state;
int rotateState();
int rotateA();
int rotateB();

int main(){
	
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;
	
	DDRC &= ~_BV(SWW);
	PORTC |= _BV(SWW);
	
	DDRC &= ~_BV(SWE);
	PORTC |= _BV(SWE);
	
	DDRC &= ~COMPASS_SWITCHES;
	PORTC |= COMPASS_SWITCHES; 
	
	DDRE &= ~_BV(SWC);
	PORTE |= _BV(SWC);
	
	DDRE &= ~_BV(ROTA);
	PORTE |= _BV(ROTA);

	DDRE &= ~_BV(ROTB);
	PORTE |= _BV(ROTB);
	
	EICRB |= _BV(ISC40) | _BV(ISC50) | _BV(ISC71);
	
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);
	
	OCR0A = (uint8_t)(F_CPU / 64 / 1000 - 0.5);
		
	TIMSK0 |= _BV(OCIE0A);	
	
	init_lcd();
    set_frame_rate_hz(61);
    set_orientation(West);
		
	state = rotateState();

	sei();
		
	while(1){

		if(changeFlag){
			//clear_screen();
			redraw();
			changeFlag=0;
		}

	}
	
	return 0;
}

ISR( TIMER0_COMPA_vect ) {
	
	if(rightPressed()){
		if(charPointer<51){
			++charPointer;
			if((int)str[charPointer]<32){
				str[charPointer]=32;
			}
			clear_screen();
			redraw();
		}
		_delay_ms(200);
	}
		
	if(leftPressed()){
		if(charPointer>0){
			--charPointer;
			if((int)str[charPointer]<32){
				str[charPointer]=32;
			}
			clear_screen();
			redraw();
		}
		_delay_ms(200);
	}
	
	if(rotateState()!=state){
		if((state==rotateState()-1) || (state==rotateState()+3)){
			state = rotateState();
			if(leftStep==2){
				if(str[charPointer]>32){
					--str[charPointer];
				}
				changeFlag=1;
				leftStep=0;
				redraw();
			}
			++leftStep;
			
		} else{
			state = rotateState();
			if(rightStep==2){
				if(str[charPointer]<126){
					++str[charPointer];
				}
				changeFlag=1;
				rightStep=0;
				redraw();
			}
			++rightStep;

		}
	}
	
}

void redraw(){
	display_string_xy(str, 5, 5);
	display_string_xy("^", 5 + (6*charPointer), 16);
}

int centrePressed(){
	if(PINE & _BV(SWC)){
		return 1;
	}
	return 0;
}

int leftPressed(){
	if(PINC & _BV(SWW)){
		return 0;
	}
	return 1;
}

int rightPressed(){
	if(PINC & _BV(SWE)){
		return 0;
	}
	return 1;
}

int rotateState(){
	if(!rotateA() && !rotateB()){
		return 0;
	} else if(rotateA() && !rotateB()){
		return 1;
	} else if(rotateA() && rotateB()){
		return 2;
	} else if(!rotateA() && rotateB()){
		return 3;
	} else {
		return 4;
	}
}

int rotateA(){
	if(PINE & _BV(ROTA)){
		return 1;
	}
	return 0;
}

int rotateB(){
	if(PINE & _BV(ROTB)){
		return 1;
	}
	return 0;
}