#include "../include/lcd.h"
#include "../include/s3c_uart.h"
#include "../include/s3c6410.h"
#include "../include/interrupt.h"
#include "../include/main.h"
#include "../include/pac.h"
#include "../include/Enemy.h"
#include "../include/mango_button.h"
#include <stdlib.h>
#include <string.h>

#define MAX_LIFE 5
#define MAX_COIN 180
extern way direct;
extern mov_stat;

int round = 4;
/* Map of pacman */
int start = 0;	// If 1 then, start mode
int biology = MAX_LIFE;
unsigned int count = 0;
int c_count = MAX_COIN;// Needed to edit

block map[20][28];
pos pacman;
pos enemy[4];

pos transform_to_pixel(pos block_pos){
	pos temp;
	temp.x = block_pos.x * 20 + 40;
	temp.y = block_pos.y * 20 + 40;
	
	return temp;
}


static int level_travel()
{
	int len;
  char buf;
 
  len = mango_uart_read(&buf, 1, 1);
  if (len > 0)
    return 0;
     
	MANGO_BTN_STATE up = mango_btn_scan(MANGO_BTN_KEYCODE_UP);
	MANGO_BTN_STATE down = mango_btn_scan(MANGO_BTN_KEYCODE_DOWN);
	MANGO_BTN_STATE left = mango_btn_scan(MANGO_BTN_KEYCODE_LEFT);
	MANGO_BTN_STATE right = mango_btn_scan(MANGO_BTN_KEYCODE_RIGHT);


  if (up == MANGO_BTN_STATE_DOWN){
						round++;
            printf("Up key pressed\n");
						return 1;
				}
       
        if (down == MANGO_BTN_STATE_DOWN){
						round--;
            printf("DOWN key pressed\n");
						return 2;
				}

        if (left == MANGO_BTN_STATE_DOWN)
            printf("LEFT key pressed\n");

        if (right == MANGO_BTN_STATE_DOWN)
            printf("RIGHT key pressed\n");


        if (up == MANGO_BTN_STATE_UP)
            printf("Up key released\n");
       
        if (down == MANGO_BTN_STATE_UP)
            printf("DOWN key released\n");

        if (left == MANGO_BTN_STATE_UP)
            printf("LEFT key released\n");

        if (right == MANGO_BTN_STATE_UP)
            printf("RIGHT key released\n");
		return 0;
}


void draw_pac(pos prev, pos cur){
	int i, incrx, incry;

	incrx = cur.x - prev.x;
	incry = cur.y - prev.y;

	pos pixel_pos_prev = transform_to_pixel(prev);
	
	for(i = 0; i < 1000; i++){
		if(i % 50 == 0){
		delete_block(pixel_pos_prev.x, pixel_pos_prev.y);
		pixel_pos_prev.x += incrx;
		pixel_pos_prev.y += incry;
		print_pacman(pixel_pos_prev.x, pixel_pos_prev.y);
	}
	}
}

void draw_enemy(pos prev[], pos cur[]){
	int i, j;
	int incrx, incry;	

	for(i = 0; i < 4; i++){
		incrx = cur[i].x - prev[i].x;
		incry = cur[i].y - prev[i].y;

		pos pixel_pos_prev = transform_to_pixel(prev[i]);
		for(j = 0; j < 20; j++){
			delete_block(pixel_pos_prev.x, pixel_pos_prev.y);
			pixel_pos_prev.x += incrx;
			pixel_pos_prev.y += incry;
			print_enemy(i, pixel_pos_prev.x, pixel_pos_prev.y);
		}
	}
}

void draw_coin(pos prev){
	pos pixel_pos_prev;

	pixel_pos_prev = transform_to_pixel(prev);
	print_coin(0xffff00, pixel_pos_prev.x, pixel_pos_prev.y);
}

void draw_map()
{
	int i, j;

	for (i = 0; i < 20; i++)
		for (j = 0; j < 28; j++)
		{
			if(map[i][j].block_type == WALL)
				print_block(0x0000ff,40+20*i,40+20*j);
			else if(map[i][j].block_type == COIN)
				print_coin(0xffff00,40+20*i,40+20*j);
			else
				print_block(0x000000,40+20*i,40+20*j);
		}
}

void main_init(void)
{
	int i, j;
	unsigned int *ph = FB_ADDR;
	for (i = 0; i < 480; i++)
		for(j = 0; j <800;j++)
			ph[i*800+j]=0x000000;
	

	/* Initiallize all the map to indicate BACK */
	for (i = 0; i < 20; i++)
		for (j = 0; j < 28; j++)
		{
			map[i][j].block_type = WALL;
		}
	
	//Positioning pacman and enemy
	pacman.x = 18;
	pacman.y = 14;
	enemy[0].x = 6;
	enemy[0].y = 14;
  enemy[1].x = 7;
  enemy[1].y = 14;
  enemy[2].x = 6;
  enemy[2].y = 15;
  enemy[3].x = 7;
  enemy[3].y = 15;
  map[6][14].block_type = BACK;
	map[7][14].block_type = BACK;
	map[6][15].block_type = BACK;
	map[7][15].block_type = BACK;

	//Positioning COIN and WALL
	coin_init();	

	// Draw controller
	print_controller();
}

void mango_menu_main(void){

	pos prev_enemy[4];
	pos prev_pacman;

	int check_valid, i;
	int level;
	if(round == 1)
		level = 50;
	else if(round == 2)
		level = 25;
	else if(round ==3)
		level = 10;
	else
		level = 2; 
	//Print first face

	startnew:
	//print map
	while(1){
	c_count = MAX_COIN;
	//Prepare new stage
	main_init();
	draw_map();
	print_stage(round);
	while(1)
	{
		if(level_travel())
			break;
		print_life(biology);	
		print_score(MAX_COIN - c_count);
		
		/* Check win */
		if(c_count == 0){
			round++;
			if(round == 5){ // clear the game
				biology = 1;
				break;
			}
			goto startnew;
		}
				
		if(count % level == 0)
			if(direct != UNDEF)
				mov_stat = direct;

		if(count % (level*2) == 0) 
		{
			//Enemy turn
			for(i = 0; i < 4; i++){
				prev_enemy[i] = enemy[i];
			}
			if(enemy_move() == -1)
				break;
			enemy_stat_modify();

			if((pacman.x == 9)&&(pacman.y==27)){
				pacman.y = 1;
			}
			if((pacman.x == 9)&&(pacman.y==0)){
				pacman.y = 26;
			}
			
			//Player turn
			prev_pacman = pacman;
			check_valid = mov_check();

			//Draw Each case
			if(check_valid == -1){ // dead case : collision with enemy
				draw_pac(prev_pacman, pacman); 
				draw_enemy(prev_enemy, enemy);
				for(i = 0; i < 4; i++){
					if(map[prev_enemy[i].x][prev_enemy[i].y].block_type == COIN)
						draw_coin(prev_enemy[i]);
				}
				break;
			}
			else if(check_valid == 1){ // collision with wall
				draw_enemy(prev_enemy, enemy); 
				for(i = 0; i < 4; i++){
					if(map[prev_enemy[i].x][prev_enemy[i].y].block_type == COIN)
						draw_coin(prev_enemy[i]);
				}
			}
			else{
				draw_pac(prev_pacman, pacman); 
				draw_enemy(prev_enemy, enemy);
				for(i = 0; i < 4; i++){
					if(map[prev_enemy[i].x][prev_enemy[i].y].block_type == COIN)
						draw_coin(prev_enemy[i]);
				}
			}
			map[pacman.x][pacman.y].block_type = BACK; 
		}

		count = ++count % 1000;

	}
	biology--;
	if(biology == 0){
		printf("Game over\n");
		break;
	}
	}
}
/* This is the main function */
int main(void){
	int c;

  /* Initillazing lcd */
  mango_uart_init(1, 115200);
	mango_btn_init();
  lcd_bl_on(MAX_BL_LEV-1);
  lcd_pwr_on();
  init_lcd_reg();
  set_lcd_pos(0, 0, S3CFB_HRES, S3CFB_VRES);
  mango_hw_init();

	/* Initiallize */
	draw_image_red();
	enable_interrupts();
	
	while(1)
	{
	  printf ("\nMain menu\n");
		printf( "1> Start Game\n");
		printf("2> Exit\n");
		c = getchar();
		printf("\n%c is selected\n", c);

		switch(c){
		case '1':
  		mango_menu_main();
			printf("Game start\n");
			break;
		case '2':
			goto finished;
		default:
			printf("Invalid button\n");
		}
	}
	finished:
  return 0;
}

void coin_init()
{
	int i;

	for(i = 1; i < 27; i++)
  	map[1][i].block_type=COIN;
	for(i = 1; i < 27; i++)
  	map[18][i].block_type=COIN;
	for(i = 1; i < 27; i++)
	  map[13][i].block_type=COIN;
	for(i = 1; i < 27; i++)
  	map[5][i].block_type=COIN;

	map[2][1].block_type=COIN;
	map[2][5].block_type=COIN;
	map[2][10].block_type=COIN;
	map[2][18].block_type=COIN;
	map[2][22].block_type=COIN;
	map[2][26].block_type=COIN;

	map[3][1].block_type=COIN;
	map[3][5].block_type=COIN;
	map[3][10].block_type=COIN;
	map[3][18].block_type=COIN;
	map[3][22].block_type=COIN;
	map[3][26].block_type=COIN;

	map[4][1].block_type=COIN;
	map[4][5].block_type=COIN;
	map[4][10].block_type=COIN;
	map[4][18].block_type=COIN;
	map[4][22].block_type=COIN;
	map[4][26].block_type=COIN;

	map[14][1].block_type=COIN;
	map[14][5].block_type=COIN;
	map[14][10].block_type=COIN;
	map[14][18].block_type=COIN;
	map[14][22].block_type=COIN;
	map[14][26].block_type=COIN;

	map[15][1].block_type=COIN;
	map[15][5].block_type=COIN;
	map[15][10].block_type=COIN;
	map[15][18].block_type=COIN;
	map[15][22].block_type=COIN;
	map[15][26].block_type=COIN;

	map[16][1].block_type=COIN;
	map[16][5].block_type=COIN;
	map[16][10].block_type=COIN;
	map[16][18].block_type=COIN;
	map[16][22].block_type=COIN;
	map[16][26].block_type=COIN;

	map[17][1].block_type=COIN;
	map[17][5].block_type=COIN;
	map[17][10].block_type=COIN;
	map[17][18].block_type=COIN;
	map[17][22].block_type=COIN;
	map[17][26].block_type=COIN;

	map[6][5].block_type=COIN;
	map[6][10].block_type=COIN;
	map[6][18].block_type=COIN;
	map[6][22].block_type=COIN;

	map[7][5].block_type=COIN;
	map[7][10].block_type=COIN;
	map[7][18].block_type=COIN;
	map[7][22].block_type=COIN;

	map[8][5].block_type=COIN;
	map[8][10].block_type=COIN;
	map[8][18].block_type=COIN;
	map[8][22].block_type=COIN;

	map[9][5].block_type=COIN;
	map[9][10].block_type=COIN;
	map[9][18].block_type=COIN;
	map[9][22].block_type=COIN;
	for(i = 0;i < 6; i++)
	  map[9][i].block_type=COIN;
	for(i = 22;i < 28; i++)
	  map[9][i].block_type=COIN;

	map[10][5].block_type=COIN;
	map[10][10].block_type=COIN;
	map[10][18].block_type=COIN;
	map[10][22].block_type=COIN;

	map[11][5].block_type=COIN;
	map[11][10].block_type=COIN;
	map[11][18].block_type=COIN;
	map[11][22].block_type=COIN;

	map[12][5].block_type=COIN;
	map[12][10].block_type=COIN;
	map[12][18].block_type=COIN;
	map[12][22].block_type=COIN;
}
