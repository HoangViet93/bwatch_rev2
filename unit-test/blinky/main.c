#include "stdint.h"
  
#define PERIPH_BASE     0x40000000
 
/* RCC peripheral addresses applicable to GPIOB */
#define RCC_BASE        (PERIPH_BASE + 0x21000)
#define RCC_APB2ENR     (*(volatile unsigned long*)(RCC_BASE + 0x18))
 
/* GPIOB peripheral addresses */
#define GPIOB_BASE      (PERIPH_BASE + 0x10C00)
#define GPIOB_CRL       (*(volatile unsigned long*)(GPIOB_BASE + 0x00))
#define GPIOB_CRH       (*(volatile unsigned long*)(GPIOB_BASE + 0x04))
#define GPIOB_BSRR      (*(volatile unsigned long*)(GPIOB_BASE + 0x10))
#define GPIOB_BRR       (*(volatile unsigned long*)(GPIOB_BASE + 0x14))
 
/* user functions */
int main(void);
void delay(unsigned long count);
  
int main()
{
    /* enable clock on GPIOB peripheral */
    RCC_APB2ENR |= 0x08;
     
    /* set LED PB2 pin output mode */
    GPIOB_CRL &= ~(0x0F << 2 * 4);
    GPIOB_CRL |= 0x03 << 2 * 4;

    while(1)
    {
		/* led on */
        GPIOB_BSRR = 1 << 2;  
        delay(200000);
		/* led off */
        GPIOB_BRR  = 1 << 2; 
        delay(200000);
    }
}
 
void delay(unsigned long count)
{
    while(count--);
}
