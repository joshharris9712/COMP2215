/*
	EXTRA WORK
	This is a program for entering text onto the display of the LaFortuna
	I have written my own rotary encoder
	
	Needs the lcd display drivers from Steve Gunn to function
*/


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
char str[51]="A"; //Initial state of the string is a single capital A that can be extended
static uint8_t changeFlag = 1, charPointer=0, leftStep=0, rightStep=0;
uint8_t state;
int rotateState();
int rotateA();
int rotateB();

int main(){
	
	//Clock config
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;
			
	//Sets compass switches to input
	DDRC &= ~COMPASS_SWITCHES;
	PORTC |= COMPASS_SWITCHES; 
	
	//Sets centre button to input
	DDRE &= ~_BV(SWC);
	PORTE |= _BV(SWC);
	
	//Sets rotary A to input
	DDRE &= ~_BV(ROTA);
	PORTE |= _BV(ROTA);

	//Sets rotary B to input
	DDRE &= ~_BV(ROTB);
	PORTE |= _BV(ROTB);
	
	//Setting up timer 0
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);
	
	//Sets the frequency of timer 0 to 1ms
	OCR0A = (uint8_t)(F_CPU / 64 / 1000 - 0.5); //F_CPU is 84,000,000
		
	//Assigning the timer register to listen on timer 0
	TIMSK0 |= _BV(OCIE0A);	
	
	//Setting up the LCD screen
	init_lcd();
    set_frame_rate_hz(61);
    set_orientation(West);
		
	//Gets the initial state of the rotary wheel
	state = rotateState();

	//Enable global interrupts
	sei();
		
	//Main program loop
	while(1){

		_delay_ms(10);
		if(changeFlag){
			redraw();
			changeFlag=0;
		}

	}
	
	return 0;
}

/*Interrupt service routine on timer 0
Used for checking if an input has been made through rotary wheel or button press*/
ISR( TIMER0_COMPA_vect ) {
	
	//Checking if the right button has been pressed
	if(rightPressed()){
		if(charPointer<51){
			//Increment the pointer to the char in the string
			++charPointer;
			//If unprintable char, change to space
			if((int)str[charPointer]<32){
				str[charPointer]=32;
			}
			changeFlag=1;
			clear_screen();
			redraw();
		}
		//delay to prevent scrolling too fast on a single press
		_delay_ms(200);
	}
	
	//Checking if the left button has been pressed
	if(leftPressed()){
		if(charPointer>0){
			//Increment the pointer to the char in the string
			--charPointer;
			//If unprintable char, change to space
			if((int)str[charPointer]<32){
				str[charPointer]=32;
			}
			changeFlag=1;
			clear_screen();
			redraw();
		}
		//delay to prevent scrolling too fast on a single press
		_delay_ms(200);
	}
	
	//Checking for a change in the rotary wheel
	if(rotateState()!=state){
		//Checking for clockwise rotation
		if((state==rotateState()-1) || (state==rotateState()+3)){
			//Recording previous state
			state = rotateState();
			//If 2 steps have been made to the left (1 click) then decrement character
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
			//Recording previous state
			state = rotateState();
			//If 2 steps have been made to the right (1 click) then increment character
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

//Draws the string over the current display 
void redraw(){
	display_string_xy(str, 5, 5);
	display_string_xy("^", 5 + (6*charPointer), 16);
}

//Detecting for centre button being pressed
int centrePressed(){
	if(PINE & _BV(SWC)){
		return 1;
	}
	return 0;
}

//Detecting for left button being pressed
int leftPressed(){
	if(PINC & _BV(SWW)){
		return 0;
	}
	return 1;
}

//Detecting for right button being pressed
int rightPressed(){
	if(PINC & _BV(SWE)){
		return 0;
	}
	return 1;
}

/*
	My implementation of the rotary encoder scanner (very verbose unlike premium german code)
*/

//Gets the current state of the rotary wheel between 0 and 3
int rotateState(){
	if(!rotateA() && !rotateB()){
		return 0;
	} else if(rotateA() && !rotateB()){
		return 1;
	} else if(rotateA() && rotateB()){
		return 2;
	} else if(!rotateA() && rotateB()){
		return 3;
	}
}

//Returns the value of rotA
int rotateA(){
	if(PINE & _BV(ROTA)){
		return 1;
	}
	return 0;
}

//Returns the value of rotB
int rotateB(){
	if(PINE & _BV(ROTB)){
		return 1;
	}
	return 0;
}