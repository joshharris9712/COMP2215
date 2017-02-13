#include  "rotary.h"
#include "lcd/lcd.c"

int cDown=0;
int state=0;

/*
	Simple code to rest rotary action
*/
int main(){
	
	init_lcd();
    set_frame_rate_hz(61);
    set_orientation(West);
	
	double pwmTime=0.5;

	DDRE &= ~_BV(SWC);
	PORTE |= _BV(SWC);
	
	DDRE &= ~_BV(ROTA);
	PORTE |= _BV(ROTA);

	DDRE &= ~_BV(ROTB);
	PORTE |= _BV(ROTB);
	
	sei();
		
	while(1){
		
		if(rotateState()!=state){
			if((state==rotateState()-1) || (state==rotateState()+3)){
				state = rotateState();
				if(pwmTime>0){
					pwmTime-=0.05;
					display_char('L');
				}
			} else{
				state = rotateState();
				if(pwmTime<1){
					pwmTime+=0.05;
					display_char('R');
				}
			}
		}

	
		PORTB |= _BV(PINB4);

		_delay_ms(pwmTime);

		PORTB &= ~_BV(PINB4);

		_delay_ms(1-pwmTime);


	}
	
	return 0;
}

int cPress(){
	if(~PINE & _BV(SWC)){
		cDown=1;
		return 1;
	} else if ((PINE & _BV(SWC)) && cDown){
		cDown=0;
	}
	return 0;
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
	} 
	return 0;
}

int rotateA(){
	if(~PINE & _BV(ROTA)){
		return 1;
	}
	return 0;
}

int rotateB(){
	if(~PINE & _BV(ROTB)){
		return 1;
	}
	return 0;
}



