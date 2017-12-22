#include "../include/lcd.h"
#include "../include/s3c_uart.h"
#include "../include/s3c6410.h"
#include "../include/interrupt.h"
#include "../include/printpacman.h"

#define MAX_LIFE 3

/* Map of pacman */
block map[20][28];

pos pacman;
pos enemy[4];

int start = 0;	// If 1 then, start mode
int life = MAX_LIFE;

typedef enum type
{
	WALL,
	BACK,
	COIN
}

typedef struct pos
{
	int x;
	int y;
}

typedef struct block
{
	type block_type;
}

void main_init(void)
{
	int i, j;
	
	/* Initiallize all the map to indicate BACK */
	for (i = 0; i < 20; i++)
		for (j = 0; j < 28; j++)
		{
			map[20][28].block_type = BACK
		}
	
	//Positioning pacman and enemy

	//Positioning COIN and WALL
}

void mango_menu_main(void){

	//Print first face

	draw_image_red();
	while(!start);
	//Run until touchpad interrupt come in

	//print map
	while(1)
	{
	// Input code here	
	
	}
}
/* This is the main function */
int main(void){

  /* Initillazing lcd */
  mango_uart_init(1, 115200);
  lcd_bl_on(MAX_BL_LEV-1);
  lcd_pwr_on();
  init_lcd_reg();
  set_lcd_pos(0, 0, S3CFB_HRES, S3CFB_VRES);
  mango_hw_init();

	/* Initiallize */
	main_init

  mango_menu_main();
  return 0;
}

