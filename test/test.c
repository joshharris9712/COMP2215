#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
int main(){
	
	while(1){
		PORTB |= _BV(PINB7);
		_delay_ms(100.0);
		PORTB &= ~_BV(PINB7);
		_delay_ms(100.0);
	}

	return 0;
}