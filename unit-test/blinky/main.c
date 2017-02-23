/*******************************
 * stm32 minimal example main.c
 *******************************/
#include "stdint.h"
  
/* memory and peripheral start addresses */
#define FLASH_BASE      0x08000000
#define SRAM_BASE       0x20000000
#define PERIPH_BASE     0x40000000
 
/* work out end of RAM address as initial stack pointer */
#define SRAM_SIZE       20*1024     // STM32F103RB has 20 Kbye of RAM
#define SRAM_END        (SRAM_BASE + SRAM_SIZE)
 
/* LED connected to PIN 8 of GPIOB */
#define LED_PIN         8
#define OUTPUT_MODE     (0x10|0x03) // output mode: push-pull + 50MHz
 
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
    uint32_t apb2enr = 0;
    uint32_t crl = 0;

    /* enable clock on GPIOB peripheral */
    RCC_APB2ENR |= 0x08;
     
    /* set LED pin output mode */
    //GPIOB_CRL |= OUTPUT_MODE << ((LED_PIN) << 2); // if pins 0 to 7
    //GPIOB_CRH |= OUTPUT_MODE << ((LED_PIN-8) << 2); // if pins 8 to 15
    GPIOB_CRL &= ~(0x0F << 2 * 4);
    GPIOB_CRL |= 0x03 << 2 * 4;

    apb2enr = RCC_APB2ENR;
    crl = GPIOB_CRL;

    while(1)
    {
        GPIOB_BSRR = 1<<2;  // set LED pin high
        delay(200000);
        GPIOB_BRR  = 1<<2;  // set LED pin low
        delay(200000);
    }
}
 
void delay(unsigned long count)
{
    while(count--);
}