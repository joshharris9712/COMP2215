#include <stdarg.h>
#include <avr/io.h>

void init(void);
void laprintf(const char *str, ...);
void inplace_reverse(char * str, int k);