#include "sub.h"

static uint8_t maxEnemies = 10;
int score=0;
volatile int enemiesX[10];
volatile int enemiesY[10];
volatile uint16_t enemiesCol[10];
volatile rectangle enemiesBox[10];
volatile float direction = 180.0;
volatile int bullet_tick=0, player_x = 50, player_y = 50, bullet_x=0, bullet_y=0;
volatile uint8_t alive=1, redraw_flag = 0, tick_count = 0, currentEnemyCount = 4, shoot_flag=0, bullet_flag=0, enemy5flag=0, enemy6flag=0, enemy7flag=0;
rectangle player_box, bullet;

int main(){
	float bullet_d = 0.0;
	init();
	
	display_sprite(sub_sprite, player_x, player_y, YELLOW);
	display_sprite(jelly_sprite, enemiesX[0], enemiesY[0], enemiesCol[0]);
	display_sprite(jelly_sprite, enemiesX[1], enemiesY[1], enemiesCol[1]);
	display_sprite(jelly_sprite, enemiesX[2], enemiesY[2], enemiesCol[2]);
	display_sprite(jelly_sprite, enemiesX[3], enemiesY[3], enemiesCol[3]);
	
	sei();
	
	while(alive){
		
		if(shoot_flag){
			bullet_d = direction;
			shoot_flag=0;
			bullet_flag=1;
		}
		
		if(bullet_flag){
			fill_rectangle(bullet, BLACK);
			bullet_x+=5*(sin(bullet_d));
			bullet_y+=5*(cos(bullet_d));
			
			if(bullet_x>=320){
				bullet_x=1;
			}
			if(bullet_y>=240){
				bullet_y=1;
			}
			if(bullet_x<=0){
				bullet_x=319;
			}
			if(bullet_y<=0){
				bullet_y=239;
			}
			bullet.left=bullet_x;
			bullet.right=bullet_x+3;
			bullet.top=bullet_y;
			bullet.bottom=bullet_y+3;
			fill_rectangle(bullet, WHITE);
		} else {
			bullet_x=0;
			bullet_y=0;
			fill_rectangle(bullet, BLACK);
		}
		
		if(redraw_flag){
			fill_rectangle(player_box, BLACK);
			player_box.left=player_x;
			player_box.right=player_x+8;
			player_box.top=player_y;
			player_box.bottom=player_y+18;
			display_sprite(sub_sprite, player_x, player_y, YELLOW);
			int i=0;
			for(i=0; i<currentEnemyCount; i++){
				fill_rectangle(enemiesBox[i], BLACK);
				enemiesBox[i].left=enemiesX[i];
				enemiesBox[i].right=enemiesX[i]+8;
				enemiesBox[i].top=enemiesY[i];
				enemiesBox[i].bottom=enemiesY[i]+14;
				display_sprite(jelly_sprite, enemiesX[i], enemiesY[i], enemiesCol[i]);
			}
			redraw_flag=0;
			checkCollision();
		}
		_delay_ms(10);

	}

	
}

void init(){
	
	DDRB  |=  _BV(PB7);
	PORTB &= ~_BV(PB7);
	
	CLKPR = (1 << CLKPCE);
	CLKPR = 1;
	
	DDRE &= ~_BV(PE4);	/* ROTB pin in */
	PORTE |= _BV(PE4);	/* ROTB off */
	
	DDRE &= ~_BV(PE5);	/* ROTA pin in */
	PORTE |= _BV(PE5);	/* ROTA off */
	
	DDRE &= ~_BV(SWC);	/* SWC pin in */
	PORTE |= _BV(SWC);	/* SWC off */
	
	TCCR0A = _BV(WGM01);
	TCCR0B = _BV(CS01) | _BV(CS00);   /* F_CPU / 64 */

	OCR0A = (uint8_t)(F_CPU / 64 / 1000 - 0.5);
	
	TIMSK0 |= _BV(OCIE0A);
	
	
	init_lcd();
    set_frame_rate_hz(61);
    set_orientation(West);
	enemiesX[0] = 200;
	enemiesX[1] = 300;
	enemiesX[2] = 100;
	enemiesX[3] = 150;
	
	enemiesY[0] = 100;
	enemiesY[1] = 150;
	enemiesY[2] = 200;
	enemiesY[3] = 20;
	
	enemiesCol[0] = BLUE;
	enemiesCol[1] = GREEN;
	enemiesCol[2] = PURPLE;
	enemiesCol[3] = RED;
	enemiesCol[4] = ORANGE;
	enemiesCol[5] = PINK;
	enemiesCol[6] = NAVY;
	int i;
	for(i=0;i<currentEnemyCount;i++){
		enemiesBox[i].left=enemiesX[i];
		enemiesBox[i].right=enemiesX[i]+8;
		enemiesBox[i].top=enemiesY[i];
		enemiesBox[i].bottom=enemiesY[i]+14;
	}
	
	player_box.left=player_x;
	player_box.right=player_x+8;
	player_box.top=player_y;
	player_box.bottom=player_y+8;
	
}

ISR( TIMER0_COMPA_vect ) {
     static int8_t last;
     int8_t new, diff;
     uint8_t wheel;
	 

     /*
        Scan rotary encoder
        ===================
        This is adapted from Peter Dannegger's code available at:
        http://www.mikrocontroller.net/articles/Drehgeber
     */

     wheel = PINE;
     new = 0;
     if( wheel  & _BV(PE4) ) new = 3;
     if( wheel  & _BV(PE5) )
	 new ^= 1;		        	/* convert gray to binary */
     diff = last - new;			/* difference last - new  */
     if( diff & 1 ){			/* bit 0 = value (1) */
	     last = new;		       	/* store new as next last  */
	     delta += (diff & 2) - 1;	/* bit 1 = direction (+/-) */
     }

	 
	 if(tick_count>10){
		float x_speed=0.0;
		float y_speed=0.0;
		tick_count=0;
		x_speed=round(2*sin((float) delta/10));
		y_speed=round(2*cos((float) delta/10)); 
		player_x+=x_speed;
		player_y+=y_speed;
		
		direction=atan2(x_speed, y_speed);
		int i;
		for(i=0;i<currentEnemyCount;i++){
			if(player_x<enemiesX[i]){
				enemiesX[i]--;
			} else {
				enemiesX[i]++;
			}
			if(player_y<enemiesY[i]){
				enemiesY[i]--;
			} else {
				enemiesY[i]++;
			}
		}
		
		checkShot();
		
		if(player_x>=320){
			player_x=1;
		}
		if(player_y>=240){
			player_y=1;
		}
		if(player_x<=0){
			player_x=319;
		}
		if(player_y<=0){
			player_y=239;
		}
		redraw_flag=1;
	 }else{
		++tick_count;
	}
	
	if(bullet_flag){
		if(bullet_tick>500){
			bullet_tick=0;
			bullet_flag=0;
		}else{
			bullet_tick++;
		}
	} else {
		if(!shoot_flag && !(_BV(SWC) & PINE)){
			shoot_flag=1;
			bullet_x = player_x;
			bullet_y = player_y;
		}
	}


}

int checkShot() {
	uint8_t i=0;
	signed int x_delta=0, y_delta=0;
	int new_x, new_y;
	for(i=0; i<currentEnemyCount; i++){
		x_delta=enemiesX[i]-bullet_x+1;
		y_delta=enemiesY[i]-bullet_y+1;
		if((x_delta<8)&&(x_delta>-8)){
			if((y_delta<12)&&(y_delta>-12)){
				score+=100;
				if(score>=1000&&!enemy5flag){
					enemiesX[4] = 5;
					enemiesY[4] = 5;
					currentEnemyCount=5;
					enemy5flag=1;
				}
				if(score>=2000&&!enemy6flag){
					enemiesX[5] = 5;
					enemiesY[5] = 5;
					currentEnemyCount=6;
					enemy6flag=1;
				}
				if(score>=3000&&!enemy7flag){
					enemiesX[6] = 5;
					enemiesY[6] = 5;
					currentEnemyCount=7;
					enemy7flag=1;
				}
				do{
					new_x = rand() % 300;
					new_y = rand() % 220;
					enemiesX[i] = new_x;
					enemiesY[i] = new_y;
					x_delta = player_x - new_x;
					y_delta = player_y - new_y;
				}while((x_delta>-50&&x_delta>50)&&(y_delta>-50&&y_delta>50));
					
				return i;
			}
		}
	}
	return 0;
}

void checkCollision() {
	uint8_t i = 0;
	signed int x_delta=0, y_delta=0;
	
	for(i=0; i<currentEnemyCount; i++){
		x_delta=enemiesX[i]-player_x;
		y_delta=enemiesY[i]-player_y-4;
		if((x_delta<8)&&(x_delta>-8)){
			if((y_delta<12)&&(y_delta>-12)){
				laprintf("GAME OVER! SCORE:%d", score);
				alive=0;
				break;
			}
		}
	}
}

	
	
