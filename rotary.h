#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SWN     PC2
#define SWE     PC3
#define SWS     PC4
#define SWW     PC5
#define SWC     PE7
#define ROTA	PE5
#define ROTB	PE4


int cPress();
int rotateA();
int rotateB();
int rotateState();