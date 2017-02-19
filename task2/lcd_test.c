/* COMP2215 15/16: Task 02---SKELETON */

#include <stdarg.h>
#include <avr/io.h>
#include "lcd.h"

void init(void);
void printf(const char *str, ...);
void inplace_reverse(char * str, int k);

void main(void) {
    init();
    display_string("Hello Southampton!\n");
    display_string("Hello UK!\n");
    display_color(GOLD, BLACK);
    display_string("Hello World!\n");
	printf("%s %couthampton! %d\n", "Hello", 'S', 1234);
}


void init(void) {
    /* 8MHz clock, no prescaling (DS, p. 48) */
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    init_lcd();
}

void printf(const char *str, ...) {
	int base = 10;
	va_list valist;
	va_start(valist, str);
	uint8_t i, j ;
	for(i=0; str[i]; i++) 
		if(str[i]=='%'){
			switch(str[i+1]){
				case 's' :
				{
					char *innerstr = va_arg(valist, char*);
					for(j=0; innerstr[j]; j++)
						display_char(innerstr[j]);
					break;
				}
				case 'd' :
				{
					int k = 0, val;
					val = va_arg(valist, int);
					char *intstr = "00000000";
					while (val != 0){
						int rem = val % base;
						intstr[k++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
						val = val/base;
					}
					intstr[k] = '\0';
					inplace_reverse(intstr, k);
					display_string(intstr);
					break;
				}
				case 'c' :
					display_char(va_arg(valist, int));
					break;
				case '%' :
					display_char('%');
					break;
			}
			++i;
		} else {
			display_char(str[i]);
		}

}


void inplace_reverse(char * str, int k)
{
  if (str)
  {
    char * end = str + k - 1;

#   define XOR_SWAP(a,b) do\
    {\
      a ^= b;\
      b ^= a;\
      a ^= b;\
    } while (0)

    while (str < end)
    {
      XOR_SWAP(*str, *end);
      str++;
      end--;
    }
#   undef XOR_SWAP
  }
}