#ifndef __INT_H
#define __INT_H

#include <stdio.h>
#include "lcd.h"
#include "s3c_uart.h"
#include "s3c6410.h"
#include "vic.h"

#define VIC0IRQSTATUS_REG __REG(ELFIN_VIC0_BASE_ADDR + 0x0)
#define VIC0INTSELECT_REG __REG(ELFIN_VIC0_BASE_ADDR + 0xc)
#define VIC0INTENABLE_REG __REG(ELFIN_VIC0_BASE_ADDR + 0x10)
#define VIC0INTENCLEAR_REG __REG(ELFIN_VIC0_BASE_ADDR + 0x14)

#define VIC0VECTADDR24 __REG(ELFIN_VIC0_BASE_ADDR + 0x160)

/* Registers for timer interrupt */
#define BIT_TIMER1 (1<<24)
#define TINT_CSTAT_REG __REG(0x7f006044)
#define BIT_TIMER1_STAT (1<<6)
#define BIT_TIMER1_EN (1<<1)

/* Registers for touch interrupt  */
#define BIT_ADCEOC (1<<31)
#define BIT_ADC_PEN (1<<30)


void enable_interrupts(void);
void disable_interrupts(void);
void timer1InterruptServiceRoutine(void);
void mango_timer_init(void);
void mango_hw_init(void);
void interrupt_reset(void);
void touch_test (void);

#endif
