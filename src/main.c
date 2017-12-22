#include "../include/lcd.h"
#include "../include/s3c_uart.h"
#include "../include/s3c6410.h"
#include "../include/interrupt.h"
#include "../include/printpacman.h"

//typedef struct block
void mango_menu_main(void){
  int c;

  while(1){
    printf ("\nMain menu\n");
    printf ("1> Enable interrupt\n");
    printf ("2> Disable interrupt\n");
    printf ("3> Touch Test\n");
    printf ("x> Exit\n");
    c = getchar();
    printf ("\n%c is selected\n", c);

    switch(c){
      case '1':
        enable_interrupts();
        printf ("interrupt enabled\n");
        break;
      case '2':
        disable_interrupts();
        printf ("interrupt disabled\n");
        break;
    case '3' :
    touch_test();
    break;
      case 'x':
        interrupt_reset();
        goto finished;
      default:
        printf ("Invalid button\n");
    }
  }
  finished:
    return;
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

	draw_image_red();

  mango_menu_main();
  return 0;
}

