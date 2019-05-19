#include "tim.h"
#include "timerdelay.h"



void TimerDelay_us(uint16_t delay_us)
{
    tim_wait = 1;
    LL_TIM_SetAutoReload(TIM_DELAY, delay_us);
    LL_TIM_SetCounter(TIM_DELAY, 0);
    LL_TIM_EnableIT_UPDATE(TIM_DELAY);
    LL_TIM_EnableCounter(TIM_DELAY);
    while(tim_wait)
    {
        __NOP();
    }
    
}
 void TIM_Delay_Callback(void)
{
    tim_wait = 0;
    if(LL_TIM_IsActiveFlag_UPDATE(TIM_DELAY))
      {
          LL_TIM_ClearFlag_UPDATE(TIM_DELAY);
          LL_TIM_DisableIT_UPDATE(TIM_DELAY);
      }
}

void TIM_Delay_Init(void)
{
    MX_TIM7_Init();
    LL_TIM_SetUpdateSource(TIM_DELAY, LL_TIM_UPDATESOURCE_COUNTER);
    LL_TIM_DisableIT_UPDATE(TIM_DELAY);
    LL_TIM_DisableCounter(TIM_DELAY);
}