#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd/lcd.c"

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
int charPointer=0;
//int rotateState();
//int rotateA();
//int rotateB();

int main(){
	
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
	
	init_lcd();
    set_frame_rate_hz(61);
    set_orientation(West);
		
	int changeFlag = 1;
	int state = rotateState();
	int leftStep=0;
	int rightStep=0;
	sei();
		
	while(1){

		if(rightPressed()){
			if(charPointer<51){
				++charPointer;
				if((int)str[charPointer]<32){
					str[charPointer]=32;
				}
				clear_screen();
				redraw();
			}
			_delay_ms(30);
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
			_delay_ms(50);
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
				}
				++rightStep;

			}
		}
		
		if(changeFlag){
			//clear_screen();
			redraw();
			changeFlag=0;
		}

	}
	
	return 0;
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