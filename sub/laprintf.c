#include "laprintf.h"

void laprintf(const char *str, ...) {
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

void laprintfxy(const char *str, int x, int y, ...) {
	char *output;
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
						output = strcat(output, innerstr[j]);
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
					output = strcat(output, intstr);
					break;
				}
				case 'c' :
					output = strcat(output, va_arg(valist, int));
					break;
				case '%' :
					output = strcat(output, '%');
					break;
			}
			++i;
		} else {
			output = strcat(output, str[i]);
		}
		output = strcat(output, '\0');
		display_string_xy(output, x, y);

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