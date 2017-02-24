#include "stm32f103xb.h"

/* This function is straight from the system_stm32f10x.c library file and
 * is called within the startup file:
 * 1. After each device reset the HSI is used as System clock source.
 * 2. This function assumes that an external 8MHz crystal is used to drive the System clock.
 */
void system_init(void)
{
    /* Reset the RCC clock configuration to the default reset state */
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;
    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= (uint32_t)0xF8FF0000;
    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;
    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;
    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = 0x009F0000;
    /* Enable HSE */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);
    /* Wait till HSE is ready */
    do
    {
    } while ((RCC->CR & RCC_CR_HSERDY) == 0);
    /* Configure the Flash Latency cycles and enable prefetch buffer */
    FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2;
    /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
    /* HCLK = SYSCLK, PCLK2 = HCLK, PCLK1 = HCLK / 2
     * If crystal is 16MHz, add in PLLXTPRE flag to prescale by 2
     */
    RCC->CFGR = (uint32_t)( RCC_CFGR_HPRE_DIV1  |
                            RCC_CFGR_PPRE2_DIV1 |
                            RCC_CFGR_PPRE1_DIV2 |
                            RCC_CFGR_PLLSRC_PREDIV1 |
                            RCC_CFGR_PLLMULL9 );
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    /* Wait till PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
    /* Select PLL as system clock source */
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08)
    {
    }
    /* Vector Table Relocation in Internal FLASH. */
    SCB->VTOR = FLASH_BASE;
}
