#include "interrupt.h"


extern int start;

//enable interrupt in CPU level
void enable_interrupts(void){
  __asm__ __volatile__ ("mrs r0, cpsr");
  __asm__ __volatile__ ("bic r0, r0, #0x80");
  __asm__ __volatile__ ("msr cpsr_c, r0");
}

//disable interrupt in CPU level
void disable_interrupts(void){
  __asm__ __volatile__ ("mrs r0, cpsr");
  __asm__ __volatile__ ("orr r0, r0, #0x80");
  __asm__ __volatile__ ("msr cpsr_c, r0");
}

unsigned int timer1_isr_call_count = 0;
int ccc = 0;
//Interrupt Service Routine for Timer1
void timer1InterruptServiceRoutine(void){
  unsigned int temp;

  //Disable any other interrupt
  temp = VIC0INTENABLE_REG;
  VIC0INTENCLEAR_REG = 0xffffffff;
	if(ccc){
		draw_image_green();
		ccc = !ccc;
	}
	else{
		draw_image_red();
		ccc = !ccc;
	}
  timer1_isr_call_count++;
  printf ("timer1InterruptSeviceRoutine is called %d times\n", timer1_isr_call_count);

  //Reset interrupt status
  TINT_CSTAT_REG |= BIT_TIMER1_STAT;
  VIC0IRQSTATUS_REG |= BIT_TIMER1;

  //Enable other interrupts
  VIC0INTENABLE_REG = temp;
}

void mango_timer_init(void){
  int i;

  TCFG0_REG = (TCFG0_REG & ~(0xff)) | 0x20 //Prescaler 0: 0x21:32
  TCFG1_REG = (TCFG1_REG & ~(0xf<<4)) | (1<<4) //divider MUX1: 1/2

  //One interrupt per one second
  TCNTB1_REG = 1000000;
  //TCMPB1_REG = 0xffff - 1;

  TCON_REG |= (1<<9); //Timer1 Manual update
  TCON_REG = (TCON_REG & ~(0xf<<8)) | (1<<11) | (1<<8);
  //Timer1 Auto-reload on & Timer1 on

  //Enable interrupt for timer1
  VIC0INTENABLE_REG |= BIT_TIMER1;
  TINT_CSTAT_REG |= BIT_TIMER1_EN;

  //Set address of interrupt handler for timer1
  VIC0VECTADDR24 = (unsigned)timer1InterruptServiceRoutine;
}

void mango_hw_init(void){
  disable_interrupts();
	mango_interrupt_init();
  mango_timer_init();
  mango_uart_init(1, 115200);
}

void interrupt_reset(void){
  VIC0INTENABLE_REG &= ~(BIT_TIMER1);
  TINT_CSTAT_REG &= ~(BIT_TIMER1_EN);
  VIC0VECTADDR24 = 0;
}

void touch_test (void) {

}

unsigned int touch_isr_call_count = 0;

void touchInterruptServiceRoutine(void){
  unsigned int temp;
  unsigned int temp2;

  if( !(VIC1RAWINTR_REG & 1<<30) )
    return;

  /* Disable any other interrupt */
  temp = VIC1INTENABLE_REG;
  VIC1INTENCLEAR_REG = 0xffffffff;

  temp2 = readl(ADCCON);
  temp2 |= 1;
  writel(temp2, ADCCON);
  writel(0xd4, ADCTSC);

  //printf ("Touch Detected\t");
  touch_isr_call_count ++;

  writel(0x1, ADCCLRINTPNDNUP);

  /* Enable other interrupts */
  VIC1INTENABLE_REG = temp;
}

void touchInterruptServiceRoutine2(void){
  unsigned int temp;
  unsigned int x, y, rx, ry;
	
	if(!start)
	{
		start = 1;
		printf("Start game!\n");
	}

  if( !(VIC1RAWINTR_REG & 1<<31) )
    return;

  /* Disable any other interrupt */
  temp = VIC1INTENABLE_REG;
  VIC1INTENCLEAR_REG = 0xffffffff;

  while( !(readl(ADCCON) & 1<<15) );
  
  x = readl(ADCDAT0) & 0x3ff;
  y = readl(ADCDAT1) & 0x3ff;

  rx = (x - 200) * 800 / 640;
  ry = (y - 340) * 480 / 360;

  printf("(%d, %d)\n", rx, ry);
 
  writel(0xd3, ADCTSC);
  writel(0x1, ADCCLRINT);
    
  /* Enable other interrupts */
  VIC1INTENABLE_REG = temp;
}

void mango_interrupt_init(void){
  VIC1INTENABLE_REG |= BIT_ADCEOC;
  VIC1INTENABLE_REG |= BIT_ADC_PEN;

  writel(0xffff, ADCDLY);
  writel(0xd3, ADCTSC);
  writel(0x7fc1, ADCCON); 

  VIC1VECTADDR30 = (unsigned)touchInterruptServiceRoutine;
  VIC1VECTADDR31 = (unsigned)touchInterruptServiceRoutine2;
}

/*
void mango_menu_main(void){
  int c;

  while(1){
		draw_image_red();
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
int main(void){

	mango_uart_init(1, 115200);
	lcd_bl_on(MAX_BL_LEV-1);
	lcd_pwr_on();
	init_lcd_reg();
	set_lcd_pos(0, 0, S3CFB_HRES, S3CFB_VRES);

  mango_hw_init();
  mango_menu_main();
  return 0;
}*/
