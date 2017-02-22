#define ERROR_HANDLER __attribute__ ((weak,  alias ("error_handler")));

extern void _estack(void);
extern void reset_handler(void);

void error_handler(void)
{
	while(1);
}

void NMI_Handler (void) ERROR_HANDLER
void HardFault_Handler (void) ERROR_HANDLER
void MemMange_Handler (void) ERROR_HANDLER
void BusFault_Handler (void) ERROR_HANDLER
void UsageFault_Handler (void) ERROR_HANDLER
void SVC_Handler (void) ERROR_HANDLER
void DebugMon_Handler (void) ERROR_HANDLER
void PendSV_Handler (void) ERROR_HANDLER
void SysTick_Handler (void) ERROR_HANDLER
void WWDG_IRQHandler (void) ERROR_HANDLER
void PVD_IRQHandler (void) ERROR_HANDLER
void TAMPER_STAMP_IRQHandler (void) ERROR_HANDLER
void RTC_WKUP_IRQHandler (void) ERROR_HANDLER
void FLASH_IRQHandler (void) ERROR_HANDLER
void RCC_IRQHandler (void) ERROR_HANDLER
void EXTI0_IRQHandler (void) ERROR_HANDLER
void EXTI1_IRQHandler (void) ERROR_HANDLER
void EXTI2_IRQHandler (void) ERROR_HANDLER
void EXTI3_IRQHandler (void) ERROR_HANDLER
void EXTI4_IRQHandler (void) ERROR_HANDLER
void DMA1_Channel1_IRQHandler (void) ERROR_HANDLER
void DMA1_Channel2_IRQHandler (void) ERROR_HANDLER
void DMA1_Channel3_IRQHandler (void) ERROR_HANDLER
void DMA1_Channel4_IRQHandler (void) ERROR_HANDLER
void DMA1_Channel5_IRQHandler (void) ERROR_HANDLER
void DMA1_Channel6_IRQHandler (void) ERROR_HANDLER
void DMA1_Channel7_IRQHandler (void) ERROR_HANDLER
void ADC1_IRQHandler (void) ERROR_HANDLER
void EXTI9_5_IRQHandler (void) ERROR_HANDLER
void TIM1_BRK_TIM15_IRQHandler (void) ERROR_HANDLER
void TIM1_UP_TIM16_IRQHandler (void) ERROR_HANDLER
void TIM1_TRG_COM_TIM17_IRQHandler (void) ERROR_HANDLER
void TIM1_CC_IRQHandler (void) ERROR_HANDLER
void TIM2_IRQHandler (void) ERROR_HANDLER
void TIM3_IRQHandler (void) ERROR_HANDLER
void TIM4_IRQHandler (void) ERROR_HANDLER
void I2C1_EV_IRQHandler (void) ERROR_HANDLER
void I2C1_ER_IRQHandler (void) ERROR_HANDLER
void I2C2_EV_IRQHandler (void) ERROR_HANDLER
void I2C2_ER_IRQHandler (void) ERROR_HANDLER
void SPI1_IRQHandler (void) ERROR_HANDLER
void SPI2_IRQHandler (void) ERROR_HANDLER
void USART1_IRQHandler (void) ERROR_HANDLER
void USART2_IRQHandler (void) ERROR_HANDLER
void USART3_IRQHandler (void) ERROR_HANDLER
void EXTI15_10_IRQHandler (void) ERROR_HANDLER
void RTCAlarm_IRQHandler (void) ERROR_HANDLER
void CEC_IRQHandler (void) ERROR_HANDLER
void TIM12_IRQHandler (void) ERROR_HANDLER
void TIM13_IRQHandler (void) ERROR_HANDLER
void TIM14_IRQHandler (void) ERROR_HANDLER
void ADC3_IRQHandler (void) ERROR_HANDLER
void FSMC_IRQHandler (void) ERROR_HANDLER
void TIM5_IRQHandler (void) ERROR_HANDLER
void SPI3_IRQHandler (void) ERROR_HANDLER
void UART4_IRQHandler (void) ERROR_HANDLER
void UART5_IRQHandler (void) ERROR_HANDLER
void TIM6_DAC_IRQHandler (void) ERROR_HANDLER
void TIM7_IRQHandler (void) ERROR_HANDLER
void DMA2_Channel1_IRQHandler (void) ERROR_HANDLER
void DMA2_Channel2_IRQHandler (void) ERROR_HANDLER
void DMA2_Channel3_IRQHandler (void) ERROR_HANDLER
void DMA2_Channel4_5_IRQHandler (void) ERROR_HANDLER
void DMA2_Channel5_IRQHandler (void) ERROR_HANDLER

__attribute__ ((section(".isr_vector")))
void (* const func_vectors[])(void) = 
{

	_estack,
	reset_handler,
	NMI_Handler,
	HardFault_Handler,
	MemMange_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0, 0, 0, 0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	PVD_IRQHandler,
	TAMPER_STAMP_IRQHandler,
	RTC_WKUP_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_IRQHandler,
	DMA1_Channel3_IRQHandler,
	DMA1_Channel4_IRQHandler,
	DMA1_Channel5_IRQHandler,
	DMA1_Channel6_IRQHandler,
	DMA1_Channel7_IRQHandler,
	ADC1_IRQHandler,
	0, 0, 0, 0,
	EXTI9_5_IRQHandler,
	TIM1_BRK_TIM15_IRQHandler,
	TIM1_UP_TIM16_IRQHandler,
	TIM1_TRG_COM_TIM17_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM4_IRQHandler,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	I2C2_EV_IRQHandler,
	I2C2_ER_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	USART3_IRQHandler,
	EXTI15_10_IRQHandler,
	RTCAlarm_IRQHandler,
	CEC_IRQHandler,
	TIM12_IRQHandler,
	TIM13_IRQHandler,
	TIM14_IRQHandler,
	0,
	ADC3_IRQHandler,
	FSMC_IRQHandler,
	0,
	TIM5_IRQHandler,
	SPI3_IRQHandler,
	UART4_IRQHandler,
	UART5_IRQHandler,
	TIM6_DAC_IRQHandler,
	TIM7_IRQHandler,
	DMA2_Channel1_IRQHandler,
	DMA2_Channel2_IRQHandler,
	DMA2_Channel3_IRQHandler,
	DMA2_Channel4_5_IRQHandler,
	DMA2_Channel5_IRQHandler,
	0, 0, 0, 0, 0, 0, 0, 0,
	0,
};