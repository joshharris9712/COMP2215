#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int8_t enc_delta();
volatile int8_t delta;