#include "tim.h"

#define TIM_DELAY  TIM7
 static volatile uint8_t tim_wait;

void TimerDelay_us(uint16_t delay_us);
void TIM_Delay_Callback(void);
void TIM_Delay_Init(void);

