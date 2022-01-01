#include "bsp_buzzer.h"

void bsp_buzzer_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    TIM_TimeBaseInitTypeDef TIM_InitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);    
       
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    //Input freq = 84Mhz
    TIM_InitStruct.TIM_Period = 279;
    TIM_InitStruct.TIM_Prescaler = 300000 / 440 - 1;
    TIM_InitStruct.TIM_ClockDivision = 0;
    TIM_InitStruct.TIM_CounterMode =TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_InitStruct);
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC3Init(TIM4, &TIM_OCInitStruct);
    TIM4->CCR3 = 140;

    TIM_Cmd(TIM4, DISABLE);
}

void bsp_buzzer_on(void)
{
    TIM_Cmd(TIM4, ENABLE);
}

void bsp_buzzer_off(void)
{
    TIM_Cmd(TIM4, DISABLE);
}

void bsp_buzzer_toggle(void)
{
    TIM4->CR1 ^= TIM_CR1_CEN;
}

void bsp_buzzer_set_freq(uint32_t freq)
{
    TIM4->PSC = 300000 / freq;
}
