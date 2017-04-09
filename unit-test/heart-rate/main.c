#include "stdio.h"
#include "stdint.h"
#include "system.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include "libopencm3/stm32/timer.h"
#include "libopencm3/cm3/nvic.h"

static void adc_init(void);
static void gpio_init(void);
static uint16_t adc_get(void);
static void timer_setup(void);

int main(void)
{
	system_init();
	gpio_init();
	adc_init();
	timer_setup();

	while (1)
	{
	}
}

static uint16_t adc_get(void)
{
	adc_start_conversion_direct(ADC1);

	/* Wait for end of conversion. */
	while (!(adc_eoc(ADC1)));

	return adc_read_regular(ADC1);
}

static void adc_init(void)
{
	uint8_t read_channel = 1;
	rcc_periph_clock_enable(RCC_ADC1);

	/* Make sure the ADC doesn't run during config. */
	adc_power_off(ADC1);

	/* We configure everything for one single conversion. */
	adc_disable_scan_mode(ADC1);
	adc_set_single_conversion_mode(ADC1);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);

	adc_power_on(ADC1);

	/* Wait for ADC starting up. */
	delay_ms(100);

	adc_reset_calibration(ADC1);
	adc_calibrate(ADC1);
	adc_set_regular_sequence(ADC1, 1, &read_channel);
}

static void gpio_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT,		
				  GPIO_CNF_INPUT_ANALOG, GPIO1);		
}

volatile int thresh = 1200;                // Ngưỡng phòng khi có nhiễu
volatile int Windows = 5000;              // Thời gian đo = 5000 x 2 ms = 10s
volatile uint32_t timer = 0;
volatile uint32_t lastBeatTime = 0;
volatile int rate[10];                    // Lưu 10 giá trị IBI gần nhất 
volatile int P = 1200;                     // Đỉnh xung - Peak
volatile int T = 1200;                     // Đáy xung - Trough
volatile int IBI = 600;                   // Thời gian liên nhịp - Inter Beat Interval
volatile int BPM = 0;      // Nhịp trên phút

volatile uint8_t Pulse = 0;           // Trả về 1 khi gặp sườn dương
volatile uint8_t Beat = 0;            // Trả về 1 khi phát hiện 1 nhịp tim

volatile int amp = 100;                   // Độ cao xung (Tính từ Trough đến Peak) - Amplitude
volatile uint8_t firstBeat = 1;        // Mảng rate[] sẽ bắt đầu bằng những giá trị IBI tin cậy
volatile uint8_t secondBeat = 1;       // Mảng rate[] sẽ bắt đầu bằng những giá trị IBI tin cậy

static void heart_rate_cal(void)
{
	uint16_t adcval = 0;

	adcval = adc_get();
	//printf("%d\t\n", adcval);
  	timer += 2;
  	Windows -= 1;
  	int N = timer - lastBeatTime;       // Tính thời gian kể từ nhịp tim gần nhất trước đó

  // Tìm đỉnh và đáy xung
  //Tìm đáy
  if((adcval < thresh) && (N > (IBI/5)*3)){   // Tránh nhiễu bằng chờ 3/5 thời gian IBI
      if (adcval < T){                        // T - Trough là đáy/bề lõm xung
          T = adcval;                         // Tìm điểm thấp nhất của xung  
       }
     }
  //Tìm đỉnh    
  if((adcval > thresh) && (adcval > P)){   // Ngưỡng thresh giúp tránh nhiễu
      P = adcval;                             // P - Peak là đỉnh xung
     }                                           // Tìm điểm cao nhất của xung

  if (N > 250){   
  	int i = 0;                                // Tránh nhiễu tần số cao gây đếm nhầm nhịp tim 
    if ( (adcval > thresh) && (Pulse == 0) && (N > (IBI/5)*3) ){        
      Pulse = 1;                               // set cờ Pulse flag mỗi khi có 1 xung
      led_b2_on();                // Bật Led ở chân D13
      IBI = timer - lastBeatTime;         // Tính thời gian giữa các nhịp đập (mS)
      lastBeatTime = timer;               // Dùng để định thời cho nhịp kế tiếp 
           
           if(firstBeat){                         // Nếu đây là nhịp tim đầu tiên, if firstBeat == 1
               firstBeat = 0;                 // Xóa cờ firstBeat
               return;                            // Giá IBI này là không đáng tin cậy vì vậy bỏ nó đi
              }   
           if(secondBeat){                        // Nếu đây là nhịp tim thứ 2, if secondBeat == 1
              secondBeat = 0;                 // Xóa cờ secondBeat
                 for(i=0; i<=9; i++){         // Bắt đầu nạp các giá trị tin cậy này vào mảng rate[]
                      rate[i] = IBI;                      
                      }
              }
            
      // keep a running total of the last 10 IBI values
      uint16_t runningTotal = 0;                  // Xóa giá trị biến tổng runningTotal   
      for(i=0; i<=8; i++){                // Dịch các giá trị trong mảng rate[]
            rate[i] = rate[i+1];              // và bỏ đi giá trị IBI cũ nhất ở phần tử rate[0]
            runningTotal += rate[i];          // tính tổng 9 giá trị IBI cũ
          }
          
      rate[9] = IBI;                          // Thêm giá trị IBI mới nhất vào phần tử cuối mảng rate[9]
      runningTotal += rate[9];                // Cộng IBI mới nhất vào biến tổng runningTotal
      runningTotal /= 10;                     // Tính trung bình 10 giá trị IBI 
      BPM = 60000/runningTotal;               // Số nhịp trong 1 phút -  BPM
      Beat = 1;                            // Trả về 1 khi có 1 nhịp
      // Beat FLAG IS NOT CLEARED INSIDE THIS ISR
      }                       
  }

  if ((adcval < thresh) && (Pulse == 1)){     // Khi tín hiệu lắng xuống 
  		led_b2_off();
      Pulse = 0;                         // reset cờ Pulse
      amp = P - T;                           // Lấy độ cao xung
      thresh = amp/2 + T;                    // Đặt ngưỡng là 50% amplitude
      P = thresh;                            // reset cho lần đo tiếp theo
      T = thresh;
     }
  
  if (N > 2500){                             // Nếu trong 2.5s mà không phát hiện nhịp tim nào
      thresh = 1200;                          // set ngưỡng thresh như mặc định
      P = 1200;                               // set ngưỡng P như mặc định
      T = 1200;                               // set ngưỡng T như mặc định
      lastBeatTime = timer;                  // Update giá trị lastBeatTime        
      firstBeat = 1;                      // Cài đặt những thông số tránh nhiễu
      secondBeat = 1;                     // cho các lần đo tiếp theo
      BPM = 0;
     }

  if(Windows == 0){
    int i = 0;
    printf("Nhip tim: ");
    printf("%d\r\n", BPM);
    printf("IBI:    ");
    for(i=0; i<10; i++){
      printf("%d", rate[i]);
      printf("\t");
      
    }
    printf("\r\n");
    Windows = 5000;     
  }
}

void timer_setup(void)
{
	rcc_periph_clock_enable(RCC_TIM2);

	rcc_periph_reset_pulse(RST_TIM2);
	
	nvic_enable_irq(NVIC_TIM2_IRQ);

	timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, 
				   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM2, ((rcc_apb1_frequency * 2) / 10000) - 1);

	timer_disable_preload(TIM2);
	timer_continuous_mode(TIM2);
	timer_set_period(TIM2, 20 - 1);
	timer_enable_counter(TIM2);
	timer_enable_irq(TIM2, TIM_DIER_UIE);
}

void tim2_isr(void)
{
	if (1 == timer_get_flag(TIM2, TIM_SR_UIF)){
		timer_clear_flag(TIM2, TIM_SR_UIF);

		heart_rate_cal();
	}
}
