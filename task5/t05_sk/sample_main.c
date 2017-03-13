/* COMP2215 15/16 Task 5---SKELETON */

#include "os.h"


int blink(int);
int update_dial(int);
int collect_delta(int);
int check_switches(int);
void laprintf(const char *str, ...);
void inplace_reverse(char * str, int k);

FIL File;  						/* FAT File */

int position = 0;

 

int main(void) {
    os_init();

    os_add_task( blink,            30, 1);
    os_add_task( collect_delta,   500, 1);
    os_add_task( check_switches,  100, 1);
     
    sei();
    for(;;){}
    
	return 0;
}


int collect_delta(int state) {
	position += os_enc_delta();
	return state;
}


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


int check_switches(int state) {
	
	if (get_switch_press(_BV(SWN))) {
			display_string("North\n");
	}
		
	if (get_switch_press(_BV(SWE))) {
			display_string("East\n");
	}
		
	if (get_switch_press(_BV(SWS))) {
			display_string("South\n");
	}
		
	if (get_switch_press(_BV(SWW))) {
			display_string("West\n");
	}
		
	if (get_switch_long(_BV(SWC))) {
		f_mount(&FatFs, "", 0);
		int res = f_open(&File, "file.txt", FA_WRITE | FA_OPEN_ALWAYS);
		laprintf("Error code: %d\n", res);
		if (res == FR_OK) {
			f_lseek(&File, f_size(&File));
			f_printf(&File, "Encoder position is: %d \r\n", position);
			f_close(&File);
			display_string("Wrote position\n");
		} else {
			display_string("Can't write file! \n");	
		}
		
	}

	if (get_switch_short(_BV(SWC))) {
			display_string("[S] Centre\n");
	}

	if (get_switch_rpt(_BV(SWN))) {
			display_string("[R] North\n");
	}
		
	if (get_switch_rpt(_BV(SWE))) {
			display_string("[R] East\n");
	}
		
	if (get_switch_rpt(_BV(SWS))) {
			display_string("[R] South\n");
	}
		
	if (get_switch_rpt(_BV(SWW))) {
			display_string("[R] West\n");
	}

	if (get_switch_rpt(SWN)) {
			display_string("[R] North\n");
	}


	if (get_switch_long(_BV(OS_CD))) {
		display_string("Detected SD card.\n");
	}

	return state;	
}




int blink(int state) {
	static int light = 0;
	uint8_t level;
	
	if (light < -120) {
		state = 1;
	} else if (light > 254) {
		state = -20;
	}


	/* Compensate somewhat for nonlinear LED 
       output and eye sensitivity:
    */
	if (state > 0) {
		if (light > 40) {
			state = 2;
		}
		if (light > 100) {
			state = 5;
		}
	} else {
		if (light < 180) {
			state = -10;
		}
		if (light < 30) {
			state = -5;
		}
	}
	light += state;

	if (light < 0) {
		level = 0;
	} else if (light > 255) {
		level = 255;
	} else {
		level = light;
	}
	
	os_led_brightness(level);
	return state;
}

